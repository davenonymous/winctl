#include "WinCtlWindow.h"

using v8::FunctionTemplate;

void InitAll(v8::Local<v8::Object> exports) {
	Window::Init(exports);

	v8::Local<v8::Context> context = exports->GetCreationContext().ToLocalChecked();
	exports->Set(context, Nan::New("GetActiveWindow").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Window::GetActiveWindow)->GetFunction(context).ToLocalChecked());
	exports->Set(context, Nan::New("GetWindowByClassName").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Window::GetWindowByClassName)->GetFunction(context).ToLocalChecked());
	exports->Set(context, Nan::New("GetWindowByTitleExact").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Window::GetWindowByTitleExact)->GetFunction(context).ToLocalChecked());
	exports->Set(context, Nan::New("EnumerateWindows").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Window::EnumerateWindows)->GetFunction(context).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)
