namespace fast_io
{
    /*
    warning:
        remove custom fastio things
    
    usage:
        https://codeforces.com/contest/659/submission/128052727
    */

	constexpr int SIZE = (1 << 21) + 1;
	int num = 0, f;
	char ibuf[SIZE], obuf[SIZE], que[65], *iS, *iT, *oS = obuf, *oT = obuf + SIZE - 1, c;
	#define gc() (iS == iT ? (iT = ((iS = ibuf) + fread(ibuf, 1, SIZE, stdin)), (iS == iT ? EOF : *iS ++)) : *iS ++)
	inline void flush()
    {
		fwrite(obuf, 1, oS - obuf, stdout);
		oS = obuf;
	}
	inline void putc(char c)
    {
		*oS ++ = c;
		if (oS == oT) flush();
	}
	inline void getc(char &c) 
    {
		for (c = gc(); (c == ' ' || c == '\0' || c == '\n') && c != EOF; c = gc());
	}
	inline void reads(char *st) 
    {
		char c;
		int n = 0; 
		getc(st[++ n]);
		for (c = gc(); c != ' ' && c != '\0' && c != '\n' && c != EOF ; c = gc()) st[++ n] = c;
		st[++ n] = '\0';
	}
	template<class I>
	inline void read(I &x) 
    {
		for (f = 1, c = gc(); c < '0' || c > '9' ; c = gc()) if (c == '-') f = -1;
		for (x = 0; c >= '0' && c <= '9' ; c = gc()) x = (x << 3) + (x << 1) + (c & 15);
		x *= f;
	}
	template<class I>
	inline void print(I x) 
    {
		if (x < 0) putc('-'), x = -x;
		if (!x) putc('0');
		while (x) que[++ num] = x % 10 + 48, x /= 10;
		while (num) putc(que[num --]);
	}
	inline void putstr(string st) 
    {
		for (int i = 0; i < (int)st.size() ; ++ i)  putc(st[i]);
	}
	struct Flusher_{~Flusher_(){flush();}} io_Flusher_;
}
using fast_io :: read;
using fast_io :: putc;
using fast_io :: putstr;
using fast_io :: getc;
using fast_io :: reads;
using fast_io :: print;