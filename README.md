TLSOpenGL Hook
==============

I have hooked OpenGL in a number of ways but haven't encountered a more stealthy approach than hooking the TLS (Thread Local Storage) stack and re-pointing the local thread's OpenGL function index addresses to new ones. Unfortunately the implementations I found were volatile as there was no automated way to find the offsets so they were being hard-coded. Fortunately after studying how OpenGL DLL itself accessed these pointers I found there was a reasonably automated way of getting these addresses by observing the OpenGL functions themselves.

Example usage
-------------

	typedef void (__stdcall *glFinish_t)();
	glFinish_t o_glFinish;
	 
	void __stdcall h_glFinish()
	{
	    MessageBox(0, "glFinish hooked!", 0, 0);
	    o_glFinish();
	}
	 
	// call from inside the main thread (e.g. in a LoadLibrary hook). Note there are other
	// ways of getting the desired threads local storage base that are more stealthy than getting
	// fs:[18] value from inside the rendering thread.
	 
	if (GLInit())
	{
	    o_glFinish = (glFinish_t) GLHook("glFinish", h_glFinish);
	}

And to unhook:

	if (o_glFinish) GLUnhook("glFinish", o_glFinish);