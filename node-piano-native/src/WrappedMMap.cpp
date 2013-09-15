/**
 * adapted from no-longer-maintained node-mmap:
 * https://github.com/bnoordhuis/node-mmap
 */

#include "WrappedMMap.h"

#include "WrapUtils.h"

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include <errno.h>
#include <unistd.h>

#include <sys/mman.h>

using namespace v8;
using namespace node;

static void unmap(char* data, void* size) {
	munmap(data, (size_t)size);
}

static Handle<Value> map(const Arguments& args) {
	HandleScope scope;

	if (args.Length() <= 5) {
		return WrapUtils::makeErrorValue(
		    "map takes 5 arguments: size, protection, flags, fd and offset.");
	}

	const size_t size = args[0]->ToInteger()->Value();
	const int protection = args[1]->ToInteger()->Value();
	const int flags = args[2]->ToInteger()->Value();
	const int fd = args[3]->ToInteger()->Value();
	const off_t offset = args[4]->ToInteger()->Value();

	char* data = (char*)mmap(0, size, protection, flags, fd, offset);

	if (data == MAP_FAILED) {
		return ThrowException(ErrnoException(errno, "mmap", ""));
	} 	else {
		Buffer* buffer = Buffer::New(data, size, unmap, (void*)size);
		return buffer->handle_;
	}
}

Handle<Value> WrappedMMap::init() {
  HandleScope scope;

  Local<Object> result = Object::New();
  
  const PropertyAttribute attribs = (PropertyAttribute) (ReadOnly | DontDelete);

  result->Set(String::New("PROT_READ"), Integer::New(PROT_READ), attribs);
  result->Set(String::New("PROT_WRITE"), Integer::New(PROT_WRITE), attribs);
  result->Set(String::New("PROT_EXEC"), Integer::New(PROT_EXEC), attribs);
  result->Set(String::New("PROT_NONE"), Integer::New(PROT_NONE), attribs);
  result->Set(String::New("MAP_SHARED"), Integer::New(MAP_SHARED), attribs);
  result->Set(String::New("MAP_PRIVATE"), Integer::New(MAP_PRIVATE), attribs);
  result->Set(String::New("PAGESIZE"), Integer::New(sysconf(_SC_PAGESIZE)),
      attribs);

  result->Set(String::NewSymbol("map"), FunctionTemplate::New(map)->GetFunction());

  return result;
}