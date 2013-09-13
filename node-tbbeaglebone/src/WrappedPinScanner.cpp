#include "WrappedPinScanner.h"
#include "WrappedPin.h"
//#include "RuntimeException.h"
#include "WrapUtils.h"

using namespace node;
using namespace std;
using namespace v8;

template <class T, class WRAPPED_T>
static inline vector<T*> unwrap(
    vector<Persistent<Value> > wrappedValues) {
  vector<T*> unwrappedValues;
  unwrappedValues.reserve(wrappedValues.size());

  for (vector<Persistent<Value> >::iterator i = wrappedValues.begin();
       i != wrappedValues.end(); ++i) {
    Persistent<Object> element((*i)->ToObject());

    T* unwrappedValue = ObjectWrap::Unwrap<WRAPPED_T>(element);
    unwrappedValues.push_back(unwrappedValue);
  }

  return unwrappedValues;
}

WrappedPinScanner::WrappedPinScanner(
    const vector<Persistent<Value> >& outputPins,
    const vector<Persistent<Value> >& inputPins)
        : PinScanner(
              unwrap<Pin, WrappedPin>(outputPins),
              unwrap<Pin, WrappedPin>(inputPins)),
          outputPins(outputPins),
          inputPins(inputPins) {
  printf("initialized!\n");
}

WrappedPinScanner::~WrappedPinScanner() {}

static inline vector<Persistent<Value> > holdArrayValuesPersistent(
    Array& array) {
  int length = array.Length();
  vector<Persistent<Value> > result;
  result.reserve(length);
  for(int i = 0; i < length; ++i) {
    Handle<Value> element = array.Get(i);
    result.push_back(Persistent<Value>(element));
  }
  return result;
}

Handle<Value> WrappedPinScanner::wrappedNew(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  // read outputPins argument
  if (argc < 1 || !args[0]->IsArray()) {
    return WrapUtils::makeErrorValue("outputPins required");
  }
  Array* outputPins = Array::Cast(*(args[0]));

  // read inputPins argument
  if (argc < 2 || !args[1]->IsArray()) {
    return WrapUtils::makeErrorValue("inputPins required");
  }
  Array* inputPins = Array::Cast(*(args[1]));

  WrappedPinScanner* instance;
  RETURN_EXCEPTIONS_AS_NODE_ERRORS({
    instance = new WrappedPinScanner(
        holdArrayValuesPersistent(*outputPins),
        holdArrayValuesPersistent(*inputPins));
  });
  instance->Wrap(args.This());
  return args.This();
}

Handle<Value> WrappedPinScanner::wrappedScan(const Arguments& args) {
  HandleScope scope;

  vector<int> results;
  CALL_WRAPPED_METHOD_WITH_RETURN(WrappedPinScanner, results, scan);

  int numResults = results.size();
  Local<Array> resultsArray = Array::New(numResults);
  for (int i = 0; i < numResults; ++i) {
    resultsArray->Set(i, Integer::New(results[i]));
  }

  return resultsArray;
}

const Persistent<FunctionTemplate>& WrappedPinScanner::constructorFunctionTemplate() {
  static Persistent<FunctionTemplate> persistentFunctionTemplate;
  static bool initialized = false;

  if (!initialized) {
    // see http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    Local<FunctionTemplate> localFunctionTemplate =
        FunctionTemplate::New(WrappedPinScanner::wrappedNew);
    persistentFunctionTemplate =
        Persistent<FunctionTemplate>::New(localFunctionTemplate);
    persistentFunctionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    persistentFunctionTemplate->SetClassName(String::NewSymbol("PinScanner"));

    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "scan", &WrappedPinScanner::wrappedScan);

    initialized = true;
  }

  return persistentFunctionTemplate;
}
