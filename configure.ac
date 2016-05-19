AC_PREREQ([2.69])
AC_INIT([MyGL], [0.00], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE(myGL, 0.00)
AC_CONFIG_SRCDIR([src/MyGL.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([GL], [glClear])
AC_CHECK_LIB([GLEW], [glewInit])
AC_CHECK_LIB([glfw], [glfwSwapBuffers])
AC_CHECK_LIB([png], [png_image_finish_read])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

#AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT(Makefile src/Makefile)
