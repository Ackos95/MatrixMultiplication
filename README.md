C++ application for multiplication matrices.

Application is written in C++ language, using Visual Studio (2015).
It implements Strassen's algorithm for matrix multiplication. Along with
serial version of algorithm, parallel version is implemented also using
Intel TBB library.

Usage of application:

	If you want to use application, first you must set up your project environment.
	Link your project to TBB library if it is not linked already. In Visual Studio
	you can do that using NuGet packages. For application testing (ApplicationTests.exe)
	you also need Google Test library (also can be managed through NuGet).

	After setting up project, you need to pass command-line arguments to app in order
	for it to work propertly. You can do that either through Visual Studio or through
	cmd.

	Also if application build is set to "Debug" change it to "Release" (in "Debug" mode
	it works very slow).

	More about command line arguments and appropriate usage you can read in application
	it self. After you run it it will print you usage instructions (unless your cmd-args
	are set propertly).

	For testing application you can run ApplicationTests project (application).

Code organisation:

	Application code is separated inside 3 libraries and two applications.
	All code and logic is inside libraries, and they are separated logically. Applications
	are only there to link all those libraries together, and to call appropriate methods
	in correct order (and handling errors).