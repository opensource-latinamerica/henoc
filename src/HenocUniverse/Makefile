OPTIONS=-O2
INCLUDES="/usr/include/malloc/"

all:
	@echo "compiling henocUniverse..."
	@g++ -c source/shapes.cpp -I. -I$(INCLUDES) -o .obj/shapes.o $(OPTIONS)
	@g++ -c source/vector.cpp -I. -I$(INCLUDES) -o .obj/vector.o $(OPTIONS)
	@g++ -c source/henocUniverse.cpp -I. -I$(INCLUDES) -o .obj/henocUniverse.o $(OPTIONS)
	@g++ -c source/intersection.cpp -I. -I$(INCLUDES) -o .obj/intersection.o $(OPTIONS)
	@g++ -c source/circle-circle.cpp -I. -I$(INCLUDES) -o .obj/circle-circle.o $(OPTIONS)
	@g++ -c source/quad-quad.cpp -I. -I$(INCLUDES) -o .obj/quad-quad.o $(OPTIONS)
	@g++ -c source/quad-circle.cpp -I. -I$(INCLUDES) -o .obj/quad-circle.o $(OPTIONS)
	@g++ -c source/terrain-all.cpp -I. -I$(INCLUDES) -o .obj/terrain-all.o $(OPTIONS)
	@g++ -c source/composite-all.cpp -I. -I$(INCLUDES) -o .obj/composite-all.o $(OPTIONS)
	@echo "compiling ODE..."
	@g++ -c ode/source/array.cpp -I. -I$(INCLUDES) -o .obj/array.o $(OPTIONS)
	@g++ -c ode/source/joint.cpp -I. -I$(INCLUDES) -o .obj/joint.o $(OPTIONS)
	@g++ -c ode/source/mass.cpp -I. -I$(INCLUDES) -o .obj/mass.o $(OPTIONS)
	@g++ -c ode/source/matrix.cpp -I. -I$(INCLUDES) -o .obj/matrix.o $(OPTIONS)
	@g++ -c ode/source/misc.cpp -I. -I$(INCLUDES) -o .obj/misc.o $(OPTIONS)
	@g++ -c ode/source/ode.cpp -I. -I$(INCLUDES) -o .obj/ode.o $(OPTIONS)
	@g++ -c ode/source/quickstep.cpp -I. -I$(INCLUDES) -o .obj/quickstep.o $(OPTIONS)
	@g++ -c ode/source/step.cpp -I. -I$(INCLUDES) -o .obj/step.o $(OPTIONS)
	@g++ -c ode/source/testing.cpp -I. -I$(INCLUDES) -o .obj/testing.o $(OPTIONS)
	@g++ -c ode/source/util.cpp -I. -I$(INCLUDES) -o .obj/util.o $(OPTIONS)
	@g++ -c ode/source/error.cpp -I. -I$(INCLUDES) -o .obj/error.o $(OPTIONS)
	@g++ -c ode/source/lcp.cpp -I. -I$(INCLUDES) -o .obj/lcp.o $(OPTIONS)
	@g++ -c ode/source/mat.cpp -I. -I$(INCLUDES) -o .obj/mat.o $(OPTIONS)
	@g++ -c ode/source/memory.cpp -I. -I$(INCLUDES) -o .obj/memory.o $(OPTIONS)
	@g++ -c ode/source/obstack.cpp -I. -I$(INCLUDES) -o .obj/obstack.o $(OPTIONS)
	@g++ -c ode/source/odemath.cpp -I. -I$(INCLUDES) -o .obj/odemath.o $(OPTIONS)
	@g++ -c ode/source/rotation.cpp -I. -I$(INCLUDES) -o .obj/rotation.o $(OPTIONS)
	@g++ -c ode/source/stepfast.cpp -I. -I$(INCLUDES) -o .obj/stepfast.o $(OPTIONS)
	@g++ -c ode/source/timer.cpp -I. -I$(INCLUDES) -o .obj/timer.o $(OPTIONS)
	@g++ -c ode/source/fastdot.c -I. -I$(INCLUDES) -o .obj/fastdot.o $(OPTIONS)
	@g++ -c ode/source/fastldlt.c -I. -I$(INCLUDES) -o .obj/fastldlt.o $(OPTIONS)
	@g++ -c ode/source/fastlsolve.c -I. -I$(INCLUDES) -o .obj/fastlsolve.o $(OPTIONS)
	@g++ -c ode/source/fastltsolve.c -I. -I$(INCLUDES) -o .obj/fastltsolve.o $(OPTIONS)
	@echo "linking libHenocUniverse..."
	@ar rcs libHenocUniverse.a .obj/shapes.o .obj/vector.o .obj/henocUniverse.o .obj/intersection.o .obj/circle-circle.o .obj/quad-quad.o .obj/quad-circle.o .obj/terrain-all.o .obj/composite-all.o .obj/array.o .obj/joint.o .obj/mass.o .obj/matrix.o .obj/misc.o .obj/ode.o .obj/quickstep.o .obj/step.o .obj/testing.o .obj/util.o .obj/error.o .obj/lcp.o .obj/mat.o .obj/memory.o .obj/obstack.o .obj/odemath.o .obj/rotation.o .obj/stepfast.o .obj/timer.o .obj/fastdot.o .obj/fastldlt.o .obj/fastlsolve.o .obj/fastltsolve.o

clean:
	@echo "cleaning henocUniverse..."
	@rm -fv .obj/*
	@rm -fv libHenocUniverse.a
	
