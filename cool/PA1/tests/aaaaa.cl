class Main inherits IO {
	str <- "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	main() : Object {
			{
					out_string("faq anyway\n");
					out_int(prod(in_int()));
					out_string("\n");
			}
	};
	prod(i : Int) : Int {
		let y : Int <- 1 in {
			while (not (i = 0) ) loop {
				out_string("faq again ");
				y <- y * in_int();
				i <- i - 1;
			}
			y;
		}
	};
}
