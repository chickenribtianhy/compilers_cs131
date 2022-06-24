class D inherits B{
	x : B <- new B;
	f(x : Int, z : String) : Int {
		1
	};
	y : Int;
};

class B inherits Dash {
	f(x : Int, z : String) : String {
		y + 2
	};
	y : Bool;
	y : Int;
};

class H inherits B {
	d(a : Bool) : Bool {
		y + 1
	};
	y : String;
};

class Main inherits IO {
	-- the class has features. Only methods in this case.


	main(): Object {
		{
			if(a < a) then 
				(out_string("Whats up?".substr(1, 3)))
			else
				out_string("Whazzy up?")
			fi;

		}

	};

	main() : Int {
		1
	};
};
