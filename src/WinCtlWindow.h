#ifndef NATIVE_EXTENSION_GRAB_W
#define NATIVE_EXTENSION_GRAB_W

#include <nan.h>
#include <windows.h>

class Window : public Nan::ObjectWrap {
public:
	explicit Window(HWND handle);
	~Window();

	static void Init(v8::Local<v8::Object> exports);
	static void GetActiveWindow(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void GetWindowByClassName(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void GetWindowByTitleExact(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void EnumerateWindows(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static Nan::Persistent<v8::Function> constructor;

private:

	static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

	static void exists(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void isVisible(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getTitle(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getHwnd(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getClassName(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getPid(const Nan::FunctionCallbackInfo<v8::Value>& info);

	static void getParent(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getAncestor(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void getMonitor(const Nan::FunctionCallbackInfo<v8::Value>& info);

	static void setForegroundWindow(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void setWindowPos(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void showWindow(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void move(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void moveRelative(const Nan::FunctionCallbackInfo<v8::Value>& info);
	static void dimensions(const Nan::FunctionCallbackInfo<v8::Value>& info);




	HWND windowHandle;
};

#endif