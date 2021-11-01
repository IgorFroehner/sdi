program PROG {
	version VERS {
		int     send(string)   = 1;
		string  receive(int)   = 2;
	} = 1;
} = 0x30001111;
