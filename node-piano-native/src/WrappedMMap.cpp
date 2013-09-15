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

  SET_INT_MEMBER_WITH_ATTRIBS(result, "PROT_READ", PROT_READ, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "PROT_WRITE", PROT_WRITE, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "PROT_EXEC", PROT_EXEC, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "PROT_NONE", PROT_NONE, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "MAP_SHARED", MAP_SHARED, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "MAP_PRIVATE", MAP_PRIVATE, attribs);
  SET_INT_MEMBER_WITH_ATTRIBS(result, "PAGESIZE", sysconf(_SC_PAGESIZE), attribs);

  SET_FUNCTION_MEMBER(result, "map", map);

  return result;
}