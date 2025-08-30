using/**/uint=unsigned/**/int;using/**/ull=unsigned/**/long/**/long;constexpr//#
/**/ull/**/lowest_bitsll[]={0/**/ull,1/**/ull,3/**/ull,7/**/ull,15/**/ull,31//##
/**/ull,63/**/ull,127/**/ull,255/**/ull,511/**/ull,1023/**/ull,2047/**/ull,//###
4095/**/ull,8191/**/ull,16383/**/ull,32767/**/ull,65535/**/ull,131071/**/ull,//#
262143/**/ull,524287/**/ull,1048575/**/ull,2097151/**/ull,4194303/**/ull,//#####
8388607/**/ull,16777215/**/ull,33554431/**/ull,67108863/**/ull,134217727/**///##
ull,268435455/**/ull,536870911/**/ull,1073741823/**/ull,2147483647/**/ull,//####
4294967295/**/ull,8589934591/**/ull,17179869183/**/ull,34359738367/**/ull,//####
68719476735/**/ull,137438953471/**/ull,274877906943/**/ull,549755813887/**/ull//
,1099511627775/**/ull,2199023255551/**/ull,4398046511103/**/ull,8796093022207//#
/**/ull,17592186044415/**/ull,35184372088831/**/ull,70368744177663/**/ull,//####
140737488355327/**/ull,281474976710655/**/ull,562949953421311/**/ull,//#########
1125899906842623/**/ull,2251799813685247/**/ull,4503599627370495/**/ull,//######
9007199254740991/**/ull,18014398509481983/**/ull,36028797018963967/**/ull,//####
72057594037927935/**/ull,144115188075855871/**/ull,288230376151711743/**/ull,//#
576460752303423487/**/ull,1152921504606846975/**/ull,2305843009213693951/**///##
ull,4611686018427387903/**/ull,9223372036854775807/**/ull,18446744073709551615//
/**/ull};const/**/uint/**/NO=1<<20;//###########################################
/* This value will be returned in lower_bound functions, if no answer exists. Change, if need.*/
template<uint/**/MAXN>/* Can correctly work with numbers in range [0; MAXN]*///#
class/**/godgod_suc_pred{static/**/const/**/uint/**/PREF=(MAXN<=64?0:MAXN<=//###
4096?1:MAXN<=262144?1+64:MAXN<=16777216?1+64+4096:MAXN<=1073741824?1+64+4096+//#
262144:227)+1;static/**/const/**/uint/**/SZ=PREF+(MAXN+63)/64+1;ull/**/m[SZ]={//
0};inline/**/uint/**/left(uint/**/v)const{return(v-62)*64;}inline/**/uint/**///#
parent(uint/**/v)const{return/**/v/64+62;}inline/**/void/**/setbit(uint/**/v){//
m[v>>6]|=1/**/ull<<(v&63);}inline/**/void/**/resetbit(uint/**/v){m[v>>6]&=~(1//#
/**/ull<<(v&63));}inline/**/uint/**/getbit(uint/**/v)const{return/**/m[v>>6]>>//
(v&63)&1;}inline/**/ull/**/childs_value(uint/**/v)const{return/**/m[left(v)>>6//
];}inline/**/uint/**/left_go(uint/**/x,const/**/uint/**/c)const{const/**/ull//##
/**/rem=x&63;uint/**/bt=PREF*64+x;ull/**/num=m[bt>>6]&lowest_bitsll[rem+c];if(//
num)return(x^rem)|__lg(num);for(bt=parent(bt);bt>62;bt=parent(bt)){const/**///##
ull/**/rem=bt&63;num=m[bt>>6]&lowest_bitsll[rem];if(num){bt=(bt^rem)|__lg(num)//
;break;}}if(bt==62)return/**/NO;while(bt<PREF*64)bt=left(bt)|__lg(m[bt-62]);//##
return/**/bt-PREF*64;}inline/**/uint/**/right_go(uint/**/x,const/**/uint/**/c)//
const{const/**/ull/**/rem=x&63;uint/**/bt=PREF*64+x;ull/**/num=m[bt>>6]&~//#####
lowest_bitsll[rem+c];if(num)return(x^rem)|__builtin_ctzll(num);for(bt=parent(//#
bt);bt>62;bt=parent(bt)){const/**/ull/**/rem=bt&63;num=m[bt>>6]&~lowest_bitsll//
[rem+1];if(num){bt=(bt^rem)|__builtin_ctzll(num);break;}}if(bt==62)return/**///#
NO;while(bt<PREF*64)bt=left(bt)|__builtin_ctzll(m[bt-62]);return/**/bt-PREF*64//
;}public:godgod_suc_pred(){assert(PREF!=228);setbit(62);}bool/**/empty()const{//
return/**/getbit(63);}void/**/clear(){fill(m,m+SZ,0);setbit(62);}bool/**/count//
(uint/**/x)const{return/**/m[PREF+(x>>6)]>>(x&63)&1;}void/**/insert(uint/**/x)//
{for(uint/**/v=PREF*64+x;!getbit(v);v=parent(v)){setbit(v);}}void/**/erase(//###
uint/**/x){if(!getbit(PREF*64+x))return;resetbit(PREF*64+x);for(uint/**/v=//####
parent(PREF*64+x);v>62&&!childs_value(v);v=parent(v)){resetbit(v);}}uint/**///##
lower_bound(uint/**/x)const{return/**/right_go(x,0);}uint/**/upper_bound(uint//#
/**/x)const{return/**/right_go(x,1);}uint/**/inverse_lower_bound(uint/**/x)//###
const{return/**/left_go(x,1);}uint/**/inverse_upper_bound(uint/**/x)const{//####
return/**/left_go(x,0);}};//####################################################
/* Supports all std::set operations in O(1) on random queries / dense arrays, O(log_64(N)) in worst case (sparce array).*/
/* Count operation works in O(1) always.*///####################################
/*  reference : https://codeforces.com/contest/1793/submission/193330718*///####
