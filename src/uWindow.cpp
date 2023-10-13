#include "UniversalUI/Window/uWindow.h"

#include "wgpu/wgpu.h"
#include <stdio.h>


void uWindow::RequestAdapterCallback(WGPURequestAdapterStatus status,
	WGPUAdapter adapter, char const* message,
	void* userdata) {
	if (status == WGPURequestAdapterStatus_Success) {
		uWindow* window = static_cast<uWindow*>(userdata);
		window->adapter = adapter;
		printf("Adapter request success!\n");
	} else {
		printf(" request_adapter status=%#.8x message=%s\n", status,
			message);
	}
}

void uWindow::RequestDeviceCallback(WGPURequestDeviceStatus status,
	WGPUDevice device, char const* message,
	void* userdata) {
	if (status == WGPURequestDeviceStatus_Success) {
		uWindow* window = static_cast<uWindow*>(userdata);
		window->device = device;
		printf("Device request success! \n");

	} else {
		printf(" request_adapter status=%#.8x message=%s\n", status,
			message);
	}
}

int uWindow::POSTResult;

#ifdef _WIN32 

	#include <Windows.h>
	PCWSTR className = L"UniversalUI Window";
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		uWindow* window = (uWindow*)data;

		switch (uMsg) {
		case WM_SIZE: {

			int width = LOWORD(lParam);    // Extract the low-order word (width)
			int height = HIWORD(lParam);   // Extract the high-order word (height)

			window->size = { (float) width, (float) height };

			if (window->forceSmoothResize) {
				// get desktop rect
				RECT desktopRect;
				GetWindowRect(GetDesktopWindow(), &desktopRect);

				if (window->displayWidth != desktopRect.right || window->displayHeight != desktopRect.bottom) {
					// monitor changed
					window->displayWidth = desktopRect.right;
					window->displayHeight = desktopRect.bottom;
					window->Resized({ (float)width, (float)height });
				}

			} else {
				window->Resized({ (float)width, (float)height });
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			window->Render();
			EndPaint(hwnd, &ps);
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		

	}

#endif // _WIN32


uWindow::uWindow(string title, uSize size) {

	this->size = size;

	//	check if post has been run
	if (POSTResult == 0) {
		#ifdef _WIN32

				WNDCLASSW wc = { 0 };

				wc.lpfnWndProc = WindowProc;
				wc.hInstance = GetModuleHandle(NULL);
				wc.lpszClassName = className;

				if (RegisterClassW(&wc) == 0) {
					POSTResult = -1;
					printf("UUI-ERROR: win32 window class registration failed.\n");
				} else {
					POSTResult = 1;
				}

		#endif // _WIN32
	}

	//	do nothing if post result failed
	if (POSTResult < 0) {
		return;
	}

	#define ASSERT_CHECK(expr)                                                     \
	  do {                                                                         \
		if (!(expr)) {                                                             \
		  int ret = EXIT_SUCCESS;                                                  \
		  printf(" assert failed %s: %s:%d\n", #expr, __FILE__,					   \
				 __LINE__);                                                        \
		  /*goto cleanup_and_exit;*/                                                   \
		}                                                                          \
	  } while (0)


	#ifdef _WIN32
		std::wstring titleWide = std::wstring(title.begin(), title.end());
		handle.rawHandle = CreateWindowExW(
			0,                              // Optional window styles.
			className,                     // Window class
			titleWide.c_str(),    // Window text
			WS_OVERLAPPEDWINDOW,            // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, (int)size.width, (int)size.height,

			NULL,       // Parent window    
			NULL,       // Menu
			GetModuleHandle(NULL),  // Instance handle
			NULL        // Additional application data
		);
		SetWindowLongPtr(handle.rawHandle, GWLP_USERDATA, (LONG_PTR)this);




		WGPUInstanceDescriptor instanceDescriptor = {};
		
		instance = wgpuCreateInstance(&instanceDescriptor);

		if (instance == NULL) {
			MessageBox(handle.rawHandle, "Failed to create WGPU instance", "Error", MB_ICONERROR);
			PostQuitMessage(1);
			return;
		}

		WGPUSurfaceDescriptor surfaceDescriptor = {};

		WGPUChainedStruct chainedStruct = {};
		chainedStruct.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;

		WGPUSurfaceDescriptorFromWindowsHWND hwndSurfaceDescriptor = {};
		hwndSurfaceDescriptor.hinstance = GetModuleHandle(NULL);
		hwndSurfaceDescriptor.hwnd = handle.rawHandle;
		hwndSurfaceDescriptor.chain = chainedStruct;

		surfaceDescriptor.nextInChain = &hwndSurfaceDescriptor.chain;

		surface = wgpuInstanceCreateSurface(instance, &surfaceDescriptor);

		
		//	WGPU ADAPTER

		WGPURequestAdapterOptions adapterOptions = {};
		adapterOptions.compatibleSurface = surface;
		adapterOptions.powerPreference = WGPUPowerPreference_HighPerformance;

		wgpuInstanceRequestAdapter(instance, &adapterOptions, RequestAdapterCallback, this);

		ASSERT_CHECK(adapter);
		

		//	WGPU DEVICE

		wgpuAdapterRequestDevice(adapter, NULL, RequestDeviceCallback, this);
		ASSERT_CHECK(device);

		// Create the initial swap chain
		RECT clientRect;
		GetClientRect(handle.rawHandle, &clientRect);
		UINT width = clientRect.right - clientRect.left;
		UINT height = clientRect.bottom - clientRect.top;

		WGPUSwapChainDescriptor swapchainDescriptor = {};
		swapchainDescriptor.usage = WGPUTextureUsage_RenderAttachment;
		swapchainDescriptor.format = WGPUTextureFormat_BGRA8Unorm;
		swapchainDescriptor.presentMode = WGPUPresentMode_Fifo;
		swapchainDescriptor.width = width;
		swapchainDescriptor.height = height;

		swapchain = wgpuDeviceCreateSwapChain(device, surface, &swapchainDescriptor);

		printf("finished the setup");

		ShowWindow(handle.rawHandle, true);
		UpdateWindow(handle.rawHandle);

#endif // _WIN32
}


void uWindow::Resized(uSize newSize) { 

	if (swapchain) {
		wgpuSwapChainRelease(swapchain);
	}

	if (surface) {
		wgpuSurfaceRelease(surface);
		surface = NULL;
	}


	WGPUSwapChainDescriptor swapchainDescriptor = {};
	swapchainDescriptor.usage = WGPUTextureUsage_RenderAttachment;
	swapchainDescriptor.format = WGPUTextureFormat_BGRA8Unorm;
	swapchainDescriptor.presentMode = WGPUPresentMode_Fifo;

	if (forceSmoothResize) {
		swapchainDescriptor.width = displayWidth;
		swapchainDescriptor.height = displayHeight;
	} else {
		swapchainDescriptor.width = (uint32_t)newSize.width;
		swapchainDescriptor.height = (uint32_t)newSize.height;
	}

	WGPUSurfaceDescriptor surfaceDescriptor = {};

	WGPUChainedStruct chainedStruct = {};
	chainedStruct.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;

	WGPUSurfaceDescriptorFromWindowsHWND hwndSurfaceDescriptor = {};
	hwndSurfaceDescriptor.hinstance = GetModuleHandle(NULL);
	hwndSurfaceDescriptor.hwnd = handle.rawHandle;
	hwndSurfaceDescriptor.chain = chainedStruct;

	surfaceDescriptor.nextInChain = &hwndSurfaceDescriptor.chain;

	surface = wgpuInstanceCreateSurface(instance, &surfaceDescriptor);

	if (surface) {
		swapchain = wgpuDeviceCreateSwapChain(device, surface, &swapchainDescriptor);
	}
	

}

void uWindow::Render() {


	// Clear the window with a blue color
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	WGPUTextureView textureView = wgpuSwapChainGetCurrentTextureView(swapchain);


	WGPURenderPassColorAttachment attachment = {};
	attachment.view = textureView;
	attachment.loadOp = WGPULoadOp_Clear;
	attachment.storeOp = WGPUStoreOp_Store;
	attachment.clearValue = { 0.0, 1.0, 0.0, 1.0 };

	WGPURenderPassDescriptor renderPassDescriptor = {};
	renderPassDescriptor.label = "Test RenderPass";
	renderPassDescriptor.colorAttachmentCount = 1;
	renderPassDescriptor.colorAttachments = { &attachment };


	WGPUCommandEncoderDescriptor encoderDescriptor = {};
	WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDescriptor);
	WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDescriptor);
	wgpuRenderPassEncoderEnd(pass);
	wgpuRenderPassEncoderRelease(pass);

	WGPUCommandBufferDescriptor bufferDesc = {};
	WGPUCommandBuffer buffer = wgpuCommandEncoderFinish(encoder, &bufferDesc);
	wgpuCommandEncoderRelease(encoder);

	WGPUQueue queue = wgpuDeviceGetQueue(device);
	wgpuQueueSubmit(queue, 1, &buffer);
	wgpuQueueRelease(queue);

	// Present the frame
	wgpuSwapChainPresent(swapchain);

}

void uWindow::PollEvents() {

#ifdef _WIN32
	MSG uMsg;
	while (PeekMessage(&uMsg, handle.rawHandle, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}

#endif // _WIN32

}