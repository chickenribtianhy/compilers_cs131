class Main inherits IO {
   newline() : Object {
	out_string("\n")
   };

   prompt() : String {
	{
	   out_string("Enter a number>");
	   in_string();
	}
   };

   main() : Object {
     (let z : A <- new B in
	while true loop  
	   (let s : String <- prompt() in
		if s = "stop" then 
		    abort() -- we don't bother to terminate gracefully
		else
		   (let i : Int <- z.a2i(s) in
			(let news : String <- z.i2a(i) in
			   {
			     newline();
			   }
	                )
                  )
		fi
	   )
        pool
     )
   };
};
