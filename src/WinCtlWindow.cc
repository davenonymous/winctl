#include "WinCtlWindow.h"
#include "windows.h"

#pragma warning ( disable:4311 )
#pragma warning ( disable:4302 )

Nan::Persistent<v8::Function> Window::constructor;

Window::Window(HWND handle) {
	this->windowHandle = handle;
}

Window::~Window() {}

void Window::Init (v8::Local<v8::Object> exports) {
	v8::Local<v8::Context> context = exports->GetCreationContext().ToLocalChecked();
	Nan::HandleScope scope;

	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Window").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "isVisible", isVisible);
	Nan::SetPrototypeMethod(tpl, "getTitle", getTitle);
	Nan::SetPrototypeMethod(tpl, "getHwnd", getHwnd);
	Nan::SetPrototypeMethod(tpl, "getClassName", getClassName);
	Nan::SetPrototypeMethod(tpl, "getPid", getPid);
	Nan::SetPrototypeMethod(tpl, "getParent", getParent);
	Nan::SetPrototypeMethod(tpl, "getAncestor", getAncestor);
	Nan::SetPrototypeMethod(tpl, "getMonitor", getMonitor);

	Nan::SetPrototypeMethod(tpl, "setForegroundWindow", setForegroundWindow);
	Nan::SetPrototypeMethod(tpl, "setWindowPos", setWindowPos);
	Nan::SetPrototypeMethod(tpl, "showWindow", showWindow);
	Nan::SetPrototypeMethod(tpl, "move", move);
	Nan::SetPrototypeMethod(tpl, "moveRelative", moveRelative);
	Nan::SetPrototypeMethod(tpl, "dimensions", dimensions);

	constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
	exports->Set(context, Nan::New("Window").ToLocalChecked(), tpl->GetFunction(context).ToLocalChecked());
}


void Window::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	if (info.IsConstructCall()) {
		Window *obj = new Window((HWND)info[0]->IntegerValue(context).FromJust());
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	} else {
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = {info[0]};
		v8::Local<v8::Function> cons = Nan::New(constructor);
		info.GetReturnValue().Set(cons->NewInstance(context, argc, argv).ToLocalChecked());
	}
}

void Window::GetActiveWindow(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	v8::Local<v8::Function> cons = Nan::New(constructor);
	const int argc = 1;
	HWND fgWin = GetForegroundWindow();
	v8::Local<v8::Value> argv[1] = {Nan::New((int)fgWin)};
	info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
}

void Window::GetWindowByClassName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	v8::Local<v8::Function> cons = Nan::New(constructor);

	v8::Isolate* isolate = info.GetIsolate();
	v8::String::Utf8Value classname(isolate, info[0]);
	std::string sClassname(*classname);

	HWND fgWin = FindWindowEx(0, 0, sClassname.c_str(), 0);

	const int argc = 1;
	v8::Local<v8::Value> argv[1] = {Nan::New((int)fgWin)};
	info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
}

void Window::GetWindowByTitleExact(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	v8::Local<v8::Function> cons = Nan::New(constructor);

	v8::Isolate* isolate = info.GetIsolate();
	v8::String::Utf8Value exactTitle(isolate, info[0]);
	std::string sExactTitle = std::string(*exactTitle);

	HWND fgWin = FindWindow(NULL, sExactTitle.c_str());

	const int argc = 1;
	v8::Local<v8::Value> argv[1] = {Nan::New((int)fgWin)};
	info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
}

v8::Local<v8::Function> enumCallback;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {

	const int constructorArgc = 1;
	v8::Local<v8::Value> constructorArgv[1] = {Nan::New((int)hwnd)};

	v8::Local<v8::Function> cons = Nan::New(Window::constructor);

	const int callbackArgc = 1;
	v8::Local<v8::Value> callbackArgv[1] = {Nan::NewInstance(cons, constructorArgc, constructorArgv).ToLocalChecked()};


	Nan::AsyncResource resource("nan:makeCallback");
	resource.runInAsyncScope(Nan::GetCurrentContext()->Global(), enumCallback, callbackArgc, callbackArgv);

	return true;
}


void Window::EnumerateWindows(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	enumCallback = info[0].As<v8::Function>();

	EnumWindows(EnumWindowsProc, 0);
}





void Window::exists(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	info.GetReturnValue().Set(Nan::New(IsWindow(obj->windowHandle)));
}

void Window::isVisible(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	info.GetReturnValue().Set(Nan::New(IsWindowVisible(obj->windowHandle)));
}


void Window::getTitle(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	char wnd_title[256];
	GetWindowText(obj->windowHandle, wnd_title, sizeof(wnd_title));

	info.GetReturnValue().Set(Nan::New(wnd_title).ToLocalChecked());
}

void Window::getHwnd(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());
	info.GetReturnValue().Set(Nan::New((int)obj->windowHandle));
}

void Window::getClassName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	char wnd_cn[256];
	GetClassName(obj->windowHandle, wnd_cn, sizeof(wnd_cn));

	info.GetReturnValue().Set(Nan::New(wnd_cn).ToLocalChecked());
}

void Window::getPid(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(obj->windowHandle, &lpdwProcessId);

	info.GetReturnValue().Set(Nan::New((int)lpdwProcessId));
}

void Window::getParent(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());
	HWND parentHwnd = GetParent(obj->windowHandle);
	if(parentHwnd != NULL) {
		info.GetReturnValue().Set(Nan::New((int)parentHwnd));
	}
}

void Window::getAncestor(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	HWND ancestorHwnd = GetAncestor(obj->windowHandle, info[0]->Int32Value(context).ToChecked());
	if(ancestorHwnd != NULL) {
		info.GetReturnValue().Set(Nan::New((int)ancestorHwnd));
	}
}

void Window::getMonitor(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());
	HMONITOR hMonitor = MonitorFromWindow(obj->windowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	v8::Local<v8::Object> result = Nan::New<v8::Object>();
	bool isPrimary = mi.dwFlags & MONITORINFOF_PRIMARY;
	Nan::Set(result, Nan::New("name").ToLocalChecked(), Nan::New(mi.szDevice).ToLocalChecked());
	Nan::Set(result, Nan::New("primary").ToLocalChecked(), Nan::New(isPrimary));

	v8::Local<v8::Object> dim = Nan::New<v8::Object>();
	Nan::Set(dim, Nan::New("left").ToLocalChecked(), Nan::New(mi.rcMonitor.left));
	Nan::Set(dim, Nan::New("top").ToLocalChecked(), Nan::New(mi.rcMonitor.top));
	Nan::Set(dim, Nan::New("right").ToLocalChecked(), Nan::New(mi.rcMonitor.right));
	Nan::Set(dim, Nan::New("bottom").ToLocalChecked(), Nan::New(mi.rcMonitor.bottom));

	Nan::Set(result, Nan::New("dimensions").ToLocalChecked(), dim);

	info.GetReturnValue().Set(result);
}














void Window::setForegroundWindow(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	SetForegroundWindow(obj->windowHandle);
}


void Window::setWindowPos(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	HWND hWndInsertAfter = (HWND)info[0]->IntegerValue(context).ToChecked();
	int X = info[1]->Int32Value(context).ToChecked();
	int Y = info[2]->Int32Value(context).ToChecked();
	int cx = info[3]->Int32Value(context).ToChecked();
	int cy = info[4]->Int32Value(context).ToChecked();
	int uFlags = info[5]->Int32Value(context).ToChecked();

	SetWindowPos(obj->windowHandle, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void Window::showWindow(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	const int nCmdShow = info[0]->Int32Value(context).ToChecked();
	ShowWindow(obj->windowHandle, nCmdShow);
}

void Window::move(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	const size_t x = info[0]->Int32Value(context).ToChecked();
	const size_t y = info[1]->Int32Value(context).ToChecked();
	const size_t w = info[2]->Int32Value(context).ToChecked();
	const size_t h = info[3]->Int32Value(context).ToChecked();

	MoveWindow(obj->windowHandle, x, y, w, h, true);
}

void Window::moveRelative(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());

	RECT dim;
	GetWindowRect(obj->windowHandle, &dim);

	const size_t dx = info[0]->Int32Value(context).ToChecked();
	const size_t dy = info[1]->Int32Value(context).ToChecked();
	const size_t dw = info[2]->Int32Value(context).ToChecked();
	const size_t dh = info[3]->Int32Value(context).ToChecked();

	const size_t x = dim.left + dx;
	const size_t y = dim.top + dy;
	const size_t w = (dim.right - dim.left) + dw;
	const size_t h = (dim.bottom - dim.top) + dh;

	MoveWindow(obj->windowHandle, x, y, w, h, true);
}


void Window::dimensions(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
	Window* obj = Nan::ObjectWrap::Unwrap<Window>(info.This());
	RECT dim;
	GetWindowRect(obj->windowHandle, &dim);

	v8::Local<v8::Object> result = Nan::New<v8::Object>();
	Nan::Set(result, Nan::New("left").ToLocalChecked(), Nan::New(dim.left));
	Nan::Set(result, Nan::New("top").ToLocalChecked(), Nan::New(dim.top));
	Nan::Set(result, Nan::New("right").ToLocalChecked(), Nan::New(dim.right));
	Nan::Set(result, Nan::New("bottom").ToLocalChecked(), Nan::New(dim.bottom));
	info.GetReturnValue().Set(result);
}
