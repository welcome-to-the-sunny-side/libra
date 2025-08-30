class/**/element_chan{public:};class/**/block_chan{public:};template<typename//#
/**/E,typename/**/T,const/**/int/**/B>class/**/block_decomposition_chan{//######
/*
        info:
            - 0 indexed
            - E is the type of elements in the array
            - T is the type of the blocks 
            - B is the block size
            - when passing in vector<E> (length = n) and vector<T>, make sure to have the size of vector<T> be at least ceil(n/B)
            - for several queries, using process() will be rather awkward, so just implement your own external function  
    */
public:int/**/ceil_div(int/**/x,int/**/y){return(x+y-1)/y;}int/**/block_id(int//
/**/i){return/**/i/B;}int/**/lb(int/**/bid){return/**/bid*B;}int/**/rb(int/**///
bid){return/**/min(n,(bid+1)*B-1);}public:int/**/n;vector<E>element;vector<T>//#
block;block_decomposition_chan(int/**/n,vector<E>a,vector<T>b):n(n),element(a)//
,block(b){};void/**/process(int/**/l,int/**/r,auto/**/block_brute,auto/**///####
block_quick){assert(1<=l/**/and/**/l<=r/**/and/**/r<=n);int/**/bl=block_id(l),//
br=block_id(r);if(bl==br)block_brute(l,r);else{block_brute(l,rb(bl));for(int//##
/**/b=bl+1;b<br;b++)block_quick(b);block_brute(lb(br),r);}}};//#################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
