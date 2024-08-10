macr macro1
main: dec x
inc *r1
endmacr  


macr macro1
.string "asvdf"
endmacr  

macr macro2 f
.string "asvdf"
endmacr 

macr
.string "asvdf"
endmacr 

macr macro1
.string "fdfd"
