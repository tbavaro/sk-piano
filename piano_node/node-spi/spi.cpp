/**
 * node-spi
 * @author tbavaro@gmail.com
 */
#include <node.h>
#include <node_buffer.h>
#include <v8.h>

#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <stdexcept>
#include <string>

#include <linux/spi/spidev.h>

// xcxc probably not needed
#include <stdio.h>

static const int BITS_PER_WORD = 8;

using namespace v8;
using namespace node;
using namespace std;

static string format(const char* fmt, ...) {
  char buffer[4096];

  va_list argptr;
  va_start(argptr, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, argptr);
  va_end(argptr);

  return buffer;
}

class Spi : node::ObjectWrap {
  public:
    Spi(const char* spiDevice, uint32_t _maxSpeedHz) 
          : fd(-1), maxSpeedHz(_maxSpeedHz) {
      fd = open(spiDevice, O_RDWR, 0);
      if (fd < 0) {
        throw runtime_error(format("unable to open SPI device: %s", spiDevice));
      }
      
      try {
        uint8_t tmp8;
        uint32_t tmp32;

        if (ioctl(fd, SPI_IOC_RD_MODE, &tmp8) == -1) {
          throw runtime_error("unable to get read mode");
        } /* else if (tmp8 != 0) {
          throw runtime_error(format("only mode 0 supported; got: %d", tmp8));
        } */

        if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp8) == -1) {
          throw runtime_error("unable to get bits per word");
        } else if (tmp8 != BITS_PER_WORD) {
          throw runtime_error(format(
                "only %d bits per word supported; got: %d", 
                BITS_PER_WORD, tmp8));
        }

        ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeedHz);

        if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32) == -1) {
          throw runtime_error("unable to get max speed");
        } else if (tmp32 != maxSpeedHz) {
          throw runtime_error(format(
              "unable to get desired max speed; wanted: %d, got: %d", 
              maxSpeedHz, tmp32));
        }
      } catch (const exception& e) {
        this->close();
        throw e;
      }
    }

    ~Spi() {
      this->close();
    }

    void close() {
      if (fd >= 0) {
        ::close(fd);
        fd = -1;
      }
    }

    void send(void* buf, int numBytes) {
      uint8_t rx_buf[numBytes];
      struct spi_ioc_transfer tr;
      tr.tx_buf = (unsigned long)buf;
      tr.rx_buf = (unsigned long)rx_buf;
      tr.len = numBytes;
      tr.delay_usecs = 5;
      tr.speed_hz = maxSpeedHz;
      tr.bits_per_word = BITS_PER_WORD;

      int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
      if (ret < 1) {
        throw runtime_error(format("unable to send SPI message: %d", ret));
      }
    }

    void wrap(Handle<Object> object) {
      this->Wrap(object);
    }

  private:
    int fd;
    uint32_t maxSpeedHz;
};

#define TYPE_ERROR(msg) \
    ThrowException(Exception::TypeError(String::New(msg)));

static Handle<Value> wrappedNew(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  // read spiDevice argument
  if (argc < 1 || !args[0]->IsString()) {
    return TYPE_ERROR("spiDevice required");
  }
  const Handle<Value>& spiDeviceArg = args[0];
  String::AsciiValue spiDevice(spiDeviceArg);

  // read maxSpeedHz argument
  if (argc < 2 || !args[1]->IsNumber()) {
    return TYPE_ERROR("maxSpeedHz required");
  }
  const Handle<Value>& maxSpeedHzArg = args[1];
  uint32_t maxSpeedHz = maxSpeedHzArg->Uint32Value();

  Spi* instance;
  try {
    instance = new Spi(*spiDevice, maxSpeedHz);
  } catch (const exception& e) {
    return TYPE_ERROR(e.what());
  }
  instance->wrap(args.This());
  return args.This();
}

static Handle<Value> wrappedClose(const Arguments& args) {
  HandleScope scope;
  Spi* instance = node::ObjectWrap::Unwrap<Spi>(args.This());
  try {        
    instance->close();
  } catch (const exception& e) {
    return TYPE_ERROR(e.what());
  }
  return Undefined();
}

static Handle<Value> wrappedSend(const Arguments& args) {
  HandleScope scope;
  Spi* instance = node::ObjectWrap::Unwrap<Spi>(args.This());
  try {        
    instance->send(NULL, 0); // xcxc
  } catch (const exception& e) {
    return TYPE_ERROR(e.what());
  }
  return Undefined();
}

static Persistent<FunctionTemplate> persistentFunctionTemplate;
static void initModule(Handle<Object> exports) {
  HandleScope scope;

  // see http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
  Local<FunctionTemplate> localFunctionTemplate = FunctionTemplate::New(wrappedNew);
  persistentFunctionTemplate = Persistent<FunctionTemplate>::New(localFunctionTemplate);
  persistentFunctionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  persistentFunctionTemplate->SetClassName(String::NewSymbol("Spi"));

  NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate, "close", wrappedClose);
  NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate, "send", wrappedSend);
  
  exports->Set(String::NewSymbol("spi"), persistentFunctionTemplate->GetFunction());
}

NODE_MODULE(spi, initModule);

