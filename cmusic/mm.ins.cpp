# 1 "mm.ins.c"
# 1 "/usr/include/string.h" 1 3
 

















 






# 1 "/usr/include/features.h" 1 3
 




















 


























































 



















 





 



 







 
# 137 "/usr/include/features.h" 3


 









 





 



























# 195 "/usr/include/features.h" 3


































 



 








 




 

# 1 "/usr/include/sys/cdefs.h" 1 3
 




















 




 





 








 




# 71 "/usr/include/sys/cdefs.h" 3


 







 



# 103 "/usr/include/sys/cdefs.h" 3



 








 















 








 








 









 







# 249 "/usr/include/features.h" 2 3


 








 





 

 








# 1 "/usr/include/gnu/stubs.h" 1 3
 




































# 277 "/usr/include/features.h" 2 3




# 26 "/usr/include/string.h" 2 3


 

 


# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 


































typedef unsigned int size_t;






















 




 

# 271 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3




 





















# 33 "/usr/include/string.h" 2 3



 
extern void *  memcpy  (void *    __dest,
			    __const void *    __src, size_t __n)    ;
 

extern void *  memmove  (void *  __dest, __const void *  __src,
			     size_t __n)    ;

 



extern void *  memccpy  (void *  __dest, __const void *  __src,
			     int __c, size_t __n)    ;



 
extern void *  memset  (void *  __s, int __c, size_t __n)    ;

 
extern int memcmp  (__const void *  __s1, __const void *  __s2,
			size_t __n)    ;

 
extern void *  memchr  (__const void *  __s, int __c, size_t __n)    ;








 
extern char *strcpy  (char *   __dest,
			  __const char *   __src)    ;
 
extern char *strncpy  (char *   __dest,
			   __const char *   __src, size_t __n)    ;

 
extern char *strcat  (char *   __dest,
			  __const char *   __src)    ;
 
extern char *strncat  (char *   __dest,
			   __const char *   __src, size_t __n)    ;

 
extern int strcmp  (__const char *__s1, __const char *__s2)    ;
 
extern int strncmp  (__const char *__s1, __const char *__s2, size_t __n)    ;

 
extern int strcoll  (__const char *__s1, __const char *__s2)    ;
 
extern size_t strxfrm  (char *   __dest,
			    __const char *   __src, size_t __n)    ;

# 107 "/usr/include/string.h" 3



 
extern char *__strdup  (__const char *__s)    ;
extern char *strdup  (__const char *__s)    ;


 






# 143 "/usr/include/string.h" 3


 
extern char *strchr  (__const char *__s, int __c)    ;
 
extern char *strrchr  (__const char *__s, int __c)    ;

 

extern size_t strcspn  (__const char *__s, __const char *__reject)    ;
 

extern size_t strspn  (__const char *__s, __const char *__accept)    ;
 
extern char *strpbrk  (__const char *__s, __const char *__accept)    ;
 
extern char *strstr  (__const char *__haystack, __const char *__needle)    ;









 
extern char *strtok  (char *   __s,
			  __const char *   __delim)    ;

 

extern char *__strtok_r  (char *   __s,
			      __const char *   __delim,
			      char **   __save_ptr)    ;

extern char *strtok_r  (char *   __s,
			    __const char *   __delim,
			    char **   __save_ptr)    ;


# 197 "/usr/include/string.h" 3



 
extern size_t strlen  (__const char *__s)    ;








 
extern char *strerror  (int __errnum)    ;

 

extern char *__strerror_r  (int __errnum, char *__buf, size_t __buflen)    ;
extern char *strerror_r  (int __errnum, char *__buf, size_t __buflen)    ;


 

extern void __bzero  (void *  __s, size_t __n)    ;


 
extern void bcopy  (__const void *  __src, void *  __dest, size_t __n)    ;

 
extern void bzero  (void *  __s, size_t __n)    ;

 
extern int bcmp  (__const void *  __s1, __const void *  __s2, size_t __n)    ;

 
extern char *index  (__const char *__s, int __c)    ;

 
extern char *rindex  (__const char *__s, int __c)    ;

 

extern int __ffs  (int __i)     __attribute__ ((const));
extern int ffs  (int __i)     __attribute__ ((const));

 









 
extern int __strcasecmp  (__const char *__s1, __const char *__s2)    ;
extern int strcasecmp  (__const char *__s1, __const char *__s2)    ;

 
extern int strncasecmp  (__const char *__s1, __const char *__s2,
			     size_t __n)    ;


# 271 "/usr/include/string.h" 3



 

extern char *strsep  (char **   __stringp,
			  __const char *   __delim)    ;


# 313 "/usr/include/string.h" 3





 

















 
# 1 "/usr/include/bits/string.h" 1 3
 






















 



 

# 782 "/usr/include/bits/string.h" 3

# 337 "/usr/include/string.h" 2 3


 
# 1 "/usr/include/bits/string2.h" 1 3
 

























 





















 
# 1 "/usr/include/endian.h" 1 3
 






















 









 
# 1 "/usr/include/bits/endian.h" 1 3
 






# 35 "/usr/include/endian.h" 2 3


 













# 50 "/usr/include/bits/string2.h" 2 3

# 1 "/usr/include/bits/types.h" 1 3
 

















 









# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3




 





















# 29 "/usr/include/bits/types.h" 2 3


 
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
# 48 "/usr/include/bits/types.h" 3

typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;

typedef __quad_t *__qaddr_t;

typedef __u_quad_t __dev_t;		 
typedef __u_int __uid_t;		 
typedef __u_int __gid_t;		 
typedef __u_long __ino_t;		 
typedef __u_int __mode_t;		 
typedef __u_int __nlink_t; 		 
typedef long int __off_t;		 
typedef __quad_t __loff_t;		 
typedef int __pid_t;			 
typedef int __ssize_t;			 
typedef long int __rlim_t;		 
typedef __quad_t __rlim64_t;		 
typedef __u_int __id_t;			 

typedef struct
  {
    int __val[2];
  } __fsid_t;				 

 
typedef int __daddr_t;			 
typedef char *__caddr_t;
typedef long int __time_t;
typedef long int __swblk_t;		 

typedef long int __clock_t;

 
typedef unsigned long int __fd_mask;

 


 




 
typedef struct
  {
     





    __fd_mask __fds_bits[1024  / (8 * sizeof (__fd_mask)) ];


  } __fd_set;


typedef int __key_t;

 
typedef unsigned short int __ipc_pid_t;


 

 
typedef __u_long __blkcnt_t;
typedef __u_quad_t __blkcnt64_t;

 
typedef long int __fsblkcnt_t;
typedef __quad_t __fsblkcnt64_t;

 
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;

 
typedef __u_long __ino64_t;

 
typedef __loff_t __off64_t;

 
typedef int __t_scalar_t;
typedef unsigned int __t_uscalar_t;

 
typedef int __intptr_t;


 





# 51 "/usr/include/bits/string2.h" 2 3











# 71 "/usr/include/bits/string2.h" 3

# 86 "/usr/include/bits/string2.h" 3


 





 








 

# 252 "/usr/include/bits/string2.h" 3



 

extern void *  __rawmemchr (const void *  __s, int __c);







 















extern __inline  char *__strcpy_small (char *, __uint16_t, __uint16_t,
				      __uint32_t, __uint32_t, size_t);
extern __inline  char *
__strcpy_small (char *__dest,
		__uint16_t __src0_2, __uint16_t __src4_2,
		__uint32_t __src0_4, __uint32_t __src4_4,
		size_t __srclen)
{
  switch (__srclen)
    {
    case 1:
      *__dest = '\0';
      break;
    case 2:
      *((__uint16_t *) __dest) = __src0_2;
      break;
    case 3:
      *((__uint16_t *) __dest) = __src0_2;
      *(__dest + 2) = '\0';
      break;
    case 4:
      *((__uint32_t *) __dest) = __src0_4;
      break;
    case 5:
      *((__uint32_t *) __dest) = __src0_4;
      *(__dest + 4) = '\0';
      break;
    case 6:
      *((__uint32_t *) __dest) = __src0_4;
      *((__uint16_t *) (__dest + 4)) = __src4_2;
      break;
    case 7:
      *((__uint32_t *) __dest) = __src0_4;
      *((__uint16_t *) (__dest + 4)) = __src4_2;
      *(__dest + 6) = '\0';
      break;
    case 8:
      *((__uint32_t *) __dest) = __src0_4;
      *((__uint32_t *) (__dest + 4)) = __src4_4;
      break;
    }
  return __dest;
}
# 395 "/usr/include/bits/string2.h" 3




 
# 542 "/usr/include/bits/string2.h" 3



 

# 557 "/usr/include/bits/string2.h" 3











 

# 578 "/usr/include/bits/string2.h" 3











 


# 611 "/usr/include/bits/string2.h" 3


# 638 "/usr/include/bits/string2.h" 3


# 660 "/usr/include/bits/string2.h" 3


# 685 "/usr/include/bits/string2.h" 3



 











 



# 717 "/usr/include/bits/string2.h" 3

extern __inline  size_t __strcspn_c1 (__const char *__s, char __reject);
extern __inline  size_t
__strcspn_c1 (__const char *__s, char __reject)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject)
    ++__result;
  return __result;
}

extern __inline  size_t __strcspn_c2 (__const char *__s, char __reject1,
				     char __reject2);
extern __inline  size_t
__strcspn_c2 (__const char *__s, char __reject1, char __reject2)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
	 && __s[__result] != __reject2)
    ++__result;
  return __result;
}

extern __inline  size_t __strcspn_c3 (__const char *__s, char __reject1,
				     char __reject2, char __reject3);
extern __inline  size_t
__strcspn_c3 (__const char *__s, char __reject1, char __reject2,
	      char __reject3)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
	 && __s[__result] != __reject2 && __s[__result] != __reject3)
    ++__result;
  return __result;
}



 



# 772 "/usr/include/bits/string2.h" 3

extern __inline  size_t __strspn_c1 (__const char *__s, char __accept);
extern __inline  size_t
__strspn_c1 (__const char *__s, char __accept)
{
  register size_t __result = 0;
   
  while (__s[__result] == __accept)
    ++__result;
  return __result;
}

extern __inline  size_t __strspn_c2 (__const char *__s, char __accept1,
				    char __accept2);
extern __inline  size_t
__strspn_c2 (__const char *__s, char __accept1, char __accept2)
{
  register size_t __result = 0;
   
  while (__s[__result] == __accept1 || __s[__result] == __accept2)
    ++__result;
  return __result;
}

extern __inline  size_t __strspn_c3 (__const char *__s, char __accept1,
				    char __accept2, char __accept3);
extern __inline  size_t
__strspn_c3 (__const char *__s, char __accept1, char __accept2, char __accept3)
{
  register size_t __result = 0;
   
  while (__s[__result] == __accept1 || __s[__result] == __accept2
	 || __s[__result] == __accept3)
    ++__result;
  return __result;
}



 


# 827 "/usr/include/bits/string2.h" 3

extern __inline  char *__strpbrk_c2 (__const char *__s, char __accept1,
				     char __accept2);
extern __inline  char *
__strpbrk_c2 (__const char *__s, char __accept1, char __accept2)
{
   
  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2)
    ++__s;
  return *__s == '\0' ? ((void *)0)  : (char *) (size_t) __s;
}

extern __inline  char *__strpbrk_c3 (__const char *__s, char __accept1,
				     char __accept2, char __accept3);
extern __inline  char *
__strpbrk_c3 (__const char *__s, char __accept1, char __accept2,
	      char __accept3)
{
   
  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2
	 && *__s != __accept3)
    ++__s;
  return *__s == '\0' ? ((void *)0)  : (char *) (size_t) __s;
}



 


# 865 "/usr/include/bits/string2.h" 3



# 878 "/usr/include/bits/string2.h" 3












extern __inline  char *__strtok_r_1c (char *__s, char __sep, char **__nextp);
extern __inline  char *
__strtok_r_1c (char *__s, char __sep, char **__nextp)
{
  char *__result;
  if (__s == ((void *)0) )
    __s = *__nextp;
  while (*__s == __sep)
    ++__s;
  if (*__s == '\0')
    __result = ((void *)0) ;
  else
    {
      __result = __s;
      while (*__s != '\0' && *__s != __sep)
	++__s;
      if (*__s == '\0')
	*__nextp = __s;
      else
	{
	  *__s = '\0';
	  *__nextp = __s + 1;
	}
    }
  return __result;
}









# 937 "/usr/include/bits/string2.h" 3

extern __inline  char *__strsep_1c (char **__s, char __reject);
extern __inline  char *
__strsep_1c (char **__s, char __reject)
{
  register char *__retval = *__s;
  if (__retval == ((void *)0) )
    return *__s = ((void *)0) ;
  if (*__retval == __reject)
    *(*__s)++ = '\0';
  else
    if ((*__s = (__extension__ (__builtin_constant_p (  __reject ) && (  __reject ) == '\0'	? (char *) __rawmemchr ( __retval ,   __reject )	: strchr ( __retval ,   __reject ))) ) != ((void *)0) )
      *(*__s)++ = '\0';
    else
      *__s = ((void *)0) ;
  return __retval;
}

extern __inline  char *__strsep_2c (char **__s, char __reject1, char __reject2);
extern __inline  char *
__strsep_2c (char **__s, char __reject1, char __reject2)
{
  register char *__retval = *__s;
  if (__retval == ((void *)0) )
    return *__s = ((void *)0) ;
  if (*__retval == __reject1 || *__retval == __reject2)
    *(*__s)++ = '\0';
  else
    {
      register char *__cp = __retval;
      while (*__cp != '\0' && *__cp != __reject1 && *__cp != __reject2)
	++__cp;
      if (*__cp != '\0')
	{
	  *__s = __cp;
	  *(*__s)++ = '\0';
	}
      else
	*__s = ((void *)0) ;
    }
  return __retval;
}

extern __inline  char *__strsep_3c (char **__s, char __reject1, char __reject2,
				   char __reject3);
extern __inline  char *
__strsep_3c (char **__s, char __reject1, char __reject2, char __reject3)
{
  register char *__retval = *__s;
  if (__retval == ((void *)0) )
    return *__s = ((void *)0) ;
  if (*__retval == __reject1 || *__retval == __reject2
      || *__retval == __reject3)
    *(*__s)++ = '\0';
  else
    {
      register char *__cp = __retval;
      while (*__cp != '\0' && *__cp != __reject1 && *__cp != __reject2
	     && *__cp != __reject3)
	++__cp;
      if (*__cp != '\0')
	{
	  *__s = __cp;
	  *(*__s)++ = '\0';
	}
      else
	*__s = ((void *)0) ;
    }
  return __retval;
}

extern __inline  char *__strsep_g (char **__s, __const char *__reject);
extern __inline  char *
__strsep_g (char **__s, __const char *__reject)
{
  register char *__retval = *__s;
  if (__retval == ((void *)0)  || *__retval == '\0')
    return ((void *)0) ;
  if ((*__s = __extension__	({ char __a0, __a1, __a2;	(__builtin_constant_p (  __reject ) && ((size_t)(const void *)((   __reject  ) + 1) - (size_t)(const void *)(   __reject  ) == 1) 	? ((__a0 = ((__const char  *) (  __reject ))[0], __a0 == '\0')	? ((void *)0) 	: ((__a1 = ((__const char *) (  __reject ))[1], __a1 == '\0')	? (__extension__ (__builtin_constant_p (  __a0 ) && (  __a0 ) == '\0'	? (char *) __rawmemchr (  __retval  ,   __a0 )	: strchr (  __retval  ,   __a0 ))) 	: ((__a2 = ((__const char *) (  __reject ))[2], __a2 == '\0')	? __strpbrk_c2 ( __retval , __a0, __a1)	: (((__const char *) (  __reject ))[3] == '\0'	? __strpbrk_c3 ( __retval , __a0, __a1, __a2)	: strpbrk ( __retval ,   __reject )))))	: strpbrk ( __retval ,   __reject )); }) ) != ((void *)0) )
    *(*__s)++ = '\0';
  return __retval;
}





 






# 1 "/usr/include/stdlib.h" 1 3
 

















 







 





# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3




 





















# 33 "/usr/include/stdlib.h" 2 3


 

# 469 "/usr/include/stdlib.h" 3




 
extern void *  malloc  (size_t __size)    ;
 
extern void *  calloc  (size_t __nmemb, size_t __size)    ;


# 768 "/usr/include/stdlib.h" 3



 


# 1031 "/usr/include/bits/string2.h" 2 3






# 1046 "/usr/include/bits/string2.h" 3









# 1072 "/usr/include/bits/string2.h" 3











# 340 "/usr/include/string.h" 2 3




 


# 1 "mm.ins.c" 2

# 1 "mm.head.h" 1
# 1 "/usr/local/include/carl/frm.h" 1 3
extern int exprerr;
float expr();
char *polish();

# 1 "mm.head.h" 2

# 1 "/usr/include/stdio.h" 1 3
 

















 









 



# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3




 





















# 33 "/usr/include/stdio.h" 2 3





# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 1 3
 





























































 






typedef void *__gnuc_va_list;



 

# 116 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 3



















# 202 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 3




# 38 "/usr/include/stdio.h" 2 3









 
typedef struct _IO_FILE FILE;








# 1 "/usr/include/libio.h" 1 3
 




























# 1 "/usr/include/_G_config.h" 1 3
 





 






# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3





 




 





























 


























typedef long int wchar_t;
























typedef unsigned int  wint_t;




 

 

# 317 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h" 3




 





















# 14 "/usr/include/_G_config.h" 2 3





















typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));




 


















 




 














# 30 "/usr/include/libio.h" 2 3
















 

# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 1 3
 





























































 










 

# 116 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 3



















# 202 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 3




# 48 "/usr/include/libio.h" 2 3







# 67 "/usr/include/libio.h" 3


 

















# 98 "/usr/include/libio.h" 3











 
























 



















struct _IO_jump_t;  struct _IO_FILE;

 







typedef void _IO_lock_t;



 

struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
   

   
  int _pos;
# 186 "/usr/include/libio.h" 3

};

struct _IO_FILE {
  int _flags;		 


   
   
  char* _IO_read_ptr;	 
  char* _IO_read_end;	 
  char* _IO_read_base;	 
  char* _IO_write_base;	 
  char* _IO_write_ptr;	 
  char* _IO_write_end;	 
  char* _IO_buf_base;	 
  char* _IO_buf_end;	 
   
  char *_IO_save_base;  
  char *_IO_backup_base;   
  char *_IO_save_end;  

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _blksize;
  __off_t   _old_offset;  


   
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

   

  _IO_lock_t *_lock;








  __off64_t   _offset;
   
  int _unused2[16];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;











# 276 "/usr/include/libio.h" 3







extern int __underflow  (_IO_FILE *)    ;
extern int __uflow  (_IO_FILE *)    ;
extern int __overflow  (_IO_FILE *, int)    ;
















extern int _IO_getc  (_IO_FILE *__fp)    ;
extern int _IO_putc  (int __c, _IO_FILE *__fp)    ;
extern int _IO_feof  (_IO_FILE *__fp)    ;
extern int _IO_ferror  (_IO_FILE *__fp)    ;

extern int _IO_peekc_locked  (_IO_FILE *__fp)    ;

 



extern void _IO_flockfile  (_IO_FILE *)    ;
extern void _IO_funlockfile  (_IO_FILE *)    ;
extern int _IO_ftrylockfile  (_IO_FILE *)    ;












extern int _IO_vfscanf  (_IO_FILE *  , const char *  ,
			     __gnuc_va_list , int *  )    ;
extern int _IO_vfprintf  (_IO_FILE *  , const char *  ,
			      __gnuc_va_list )    ;
extern __ssize_t   _IO_padn  (_IO_FILE *, int, __ssize_t  )    ;
extern size_t   _IO_sgetn  (_IO_FILE *, void *, size_t  )    ;

extern __off64_t   _IO_seekoff  (_IO_FILE *, __off64_t  , int, int)    ;
extern __off64_t   _IO_seekpos  (_IO_FILE *, __off64_t  , int)    ;

extern void _IO_free_backup_area  (_IO_FILE *)    ;






# 57 "/usr/include/stdio.h" 2 3


 

typedef __off_t  fpos_t;







 





 





 






 







 




 








# 1 "/usr/include/bits/stdio_lim.h" 1 3
 




































# 110 "/usr/include/stdio.h" 2 3



 
extern FILE *stdin;		 
extern FILE *stdout;		 
extern FILE *stderr;		 


 
extern int remove  (__const char *__filename)    ;
 
extern int rename  (__const char *__old, __const char *__new)    ;


 

extern FILE *tmpfile  (void)    ;










 
extern char *tmpnam  (char *__s)    ;


 

extern char *tmpnam_r  (char *__s)    ;




 






extern char *tempnam  (__const char *__dir, __const char *__pfx)    ;



 
extern int fclose  (FILE *__stream)    ;
 
extern int fflush  (FILE *__stream)    ;


 
extern int fflush_unlocked  (FILE *__stream)    ;









 
extern FILE *fopen  (__const char *   __filename,
			 __const char *   __modes)    ;
 
extern FILE *freopen  (__const char *   __filename,
			   __const char *   __modes,
			   FILE *   __stream)    ;
# 197 "/usr/include/stdio.h" 3










 
extern FILE *fdopen  (int __fd, __const char *__modes)    ;


# 223 "/usr/include/stdio.h" 3



 

extern void setbuf  (FILE *   __stream, char *   __buf)    ;
 


extern int setvbuf  (FILE *   __stream, char *   __buf,
			 int __modes, size_t __n)    ;


 

extern void setbuffer  (FILE *   __stream, char *   __buf,
			    size_t __size)    ;

 
extern void setlinebuf  (FILE *__stream)    ;



 
extern int fprintf  (FILE *   __stream,
			 __const char *   __format, ...)    ;
 
extern int printf  (__const char *   __format, ...)    ;
 
extern int sprintf  (char *   __s,
			 __const char *   __format, ...)    ;

 
extern int vfprintf  (FILE *   __s,
			  __const char *   __format,
			  __gnuc_va_list  __arg)    ;
 
extern int vprintf  (__const char *   __format,
			 __gnuc_va_list  __arg)    ;
 
extern int vsprintf  (char *   __s,
			  __const char *   __format,
			  __gnuc_va_list  __arg)    ;


 
extern int snprintf  (char *   __s, size_t __maxlen,
			  __const char *   __format, ...)    
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern int __vsnprintf  (char *   __s, size_t __maxlen,
			     __const char *   __format,
			     __gnuc_va_list  __arg)    
     __attribute__ ((__format__ (__printf__, 3, 0)));
extern int vsnprintf  (char *   __s, size_t __maxlen,
			   __const char *   __format,
			   __gnuc_va_list  __arg)    
     __attribute__ ((__format__ (__printf__, 3, 0)));


# 302 "/usr/include/stdio.h" 3



 
extern int fscanf  (FILE *   __stream,
			__const char *   __format, ...)    ;
 
extern int scanf  (__const char *   __format, ...)    ;
 
extern int sscanf  (__const char *   __s,
			__const char *   __format, ...)    ;

# 330 "/usr/include/stdio.h" 3



 
extern int fgetc  (FILE *__stream)    ;
extern int getc  (FILE *__stream)    ;

 
extern int getchar  (void)    ;

 




 
extern int getc_unlocked  (FILE *__stream)    ;
extern int getchar_unlocked  (void)    ;



 
extern int fgetc_unlocked  (FILE *__stream)    ;



 
extern int fputc  (int __c, FILE *__stream)    ;
extern int putc  (int __c, FILE *__stream)    ;

 
extern int putchar  (int __c)    ;

 




 
extern int fputc_unlocked  (int __c, FILE *__stream)    ;



 
extern int putc_unlocked  (int __c, FILE *__stream)    ;
extern int putchar_unlocked  (int __c)    ;




 
extern int getw  (FILE *__stream)    ;

 
extern int putw  (int __w, FILE *__stream)    ;



 
extern char *fgets  (char *   __s, int __n,
			 FILE *   __stream)    ;







 

extern char *gets  (char *__s)    ;


# 420 "/usr/include/stdio.h" 3



 
extern int fputs  (__const char *   __s,
		       FILE *   __stream)    ;







 
extern int puts  (__const char *__s)    ;


 
extern int ungetc  (int __c, FILE *__stream)    ;


 
extern size_t fread  (void *   __ptr, size_t __size,
			  size_t __n, FILE *   __stream)    ;
 
extern size_t fwrite  (__const void *   __ptr, size_t __size,
			   size_t __n, FILE *   __s)    ;


 
extern size_t fread_unlocked  (void *   __ptr, size_t __size,
				   size_t __n, FILE *   __stream)    ;
extern size_t fwrite_unlocked  (__const void *   __ptr,
				    size_t __size, size_t __n,
				    FILE *   __stream)    ;



 
extern int fseek  (FILE *__stream, long int __off, int __whence)    ;
 
extern long int ftell  (FILE *__stream)    ;
 
extern void rewind  (FILE *__stream)    ;

 




 


typedef __off_t off_t;




















 
extern int fgetpos  (FILE *   __stream,
			 fpos_t *   __pos)    ;
 
extern int fsetpos  (FILE *__stream, __const fpos_t *__pos)    ;
# 519 "/usr/include/stdio.h" 3


# 529 "/usr/include/stdio.h" 3


 
extern void clearerr  (FILE *__stream)    ;
 
extern int feof  (FILE *__stream)    ;
 
extern int ferror  (FILE *__stream)    ;


 
extern void clearerr_unlocked  (FILE *__stream)    ;
extern int feof_unlocked  (FILE *__stream)    ;
extern int ferror_unlocked  (FILE *__stream)    ;



 
extern void perror  (__const char *__s)    ;

 


extern int sys_nerr;
extern __const char *__const sys_errlist[];








 
extern int fileno  (FILE *__stream)    ;



 
extern int fileno_unlocked  (FILE *__stream)    ;





 
extern FILE *popen  (__const char *__command, __const char *__modes)    ;

 
extern int pclose  (FILE *__stream)    ;




 
extern char *ctermid  (char *__s)    ;









# 603 "/usr/include/stdio.h" 3




 

 
extern void flockfile  (FILE *__stream)    ;

 

extern int ftrylockfile  (FILE *__stream)    ;

 
extern void funlockfile  (FILE *__stream)    ;










 


# 1 "/usr/include/bits/stdio.h" 1 3
 





























 
extern __inline  int
vprintf (__const char *   __fmt, __gnuc_va_list  __arg)  
{
  return vfprintf (stdout, __fmt, __arg);
}

 
extern __inline  int
getchar (void)  
{
  return _IO_getc (stdin);
}



 
extern __inline  int
getc_unlocked (FILE *__fp)  
{
  return (( __fp )->_IO_read_ptr >= ( __fp )->_IO_read_end ? __uflow ( __fp ) : *(unsigned char *) ( __fp )->_IO_read_ptr++) ;
}

 
extern __inline  int
getchar_unlocked (void)  
{
  return (( stdin )->_IO_read_ptr >= ( stdin )->_IO_read_end ? __uflow ( stdin ) : *(unsigned char *) ( stdin )->_IO_read_ptr++) ;
}



 
extern __inline  int
putchar (int __c)  
{
  return _IO_putc (__c, stdout);
}



 
extern __inline  int
fputc_unlocked (int __c, FILE *__stream)  
{
  return (((  __stream )->_IO_write_ptr >= (  __stream )->_IO_write_end) ? __overflow (  __stream , (unsigned char) ( __c )) : (unsigned char) (*(  __stream )->_IO_write_ptr++ = ( __c ))) ;
}




 
extern __inline  int
putc_unlocked (int __c, FILE *__stream)  
{
  return (((  __stream )->_IO_write_ptr >= (  __stream )->_IO_write_end) ? __overflow (  __stream , (unsigned char) ( __c )) : (unsigned char) (*(  __stream )->_IO_write_ptr++ = ( __c ))) ;
}

 
extern __inline  int
putchar_unlocked (int __c)  
{
  return (((  stdout )->_IO_write_ptr >= (  stdout )->_IO_write_end) ? __overflow (  stdout , (unsigned char) ( __c )) : (unsigned char) (*(  stdout )->_IO_write_ptr++ = ( __c ))) ;
}














 
extern __inline  int
feof_unlocked (FILE *__stream)  
{
  return ((( __stream )->_flags & 0x10 ) != 0) ;
}

 
extern __inline  int
ferror_unlocked (FILE *__stream)  
{
  return ((( __stream )->_flags & 0x20 ) != 0) ;
}






 

# 149 "/usr/include/bits/stdio.h" 3


# 165 "/usr/include/bits/stdio.h" 3


 

# 631 "/usr/include/stdio.h" 2 3



 




# 2 "mm.head.h" 2

# 1 "/usr/include/ctype.h" 1 3
 

















 









 


 














enum
{
  _ISupper = (( 0 ) < 8 ? ((1 << ( 0 )) << 8) : ((1 << ( 0 )) >> 8)) ,	 
  _ISlower = (( 1 ) < 8 ? ((1 << ( 1 )) << 8) : ((1 << ( 1 )) >> 8)) ,	 
  _ISalpha = (( 2 ) < 8 ? ((1 << ( 2 )) << 8) : ((1 << ( 2 )) >> 8)) ,	 
  _ISdigit = (( 3 ) < 8 ? ((1 << ( 3 )) << 8) : ((1 << ( 3 )) >> 8)) ,	 
  _ISxdigit = (( 4 ) < 8 ? ((1 << ( 4 )) << 8) : ((1 << ( 4 )) >> 8)) ,	 
  _ISspace = (( 5 ) < 8 ? ((1 << ( 5 )) << 8) : ((1 << ( 5 )) >> 8)) ,	 
  _ISprint = (( 6 ) < 8 ? ((1 << ( 6 )) << 8) : ((1 << ( 6 )) >> 8)) ,	 
  _ISgraph = (( 7 ) < 8 ? ((1 << ( 7 )) << 8) : ((1 << ( 7 )) >> 8)) ,	 
  _ISblank = (( 8 ) < 8 ? ((1 << ( 8 )) << 8) : ((1 << ( 8 )) >> 8)) ,	 
  _IScntrl = (( 9 ) < 8 ? ((1 << ( 9 )) << 8) : ((1 << ( 9 )) >> 8)) ,	 
  _ISpunct = (( 10 ) < 8 ? ((1 << ( 10 )) << 8) : ((1 << ( 10 )) >> 8)) ,	 
  _ISalnum = (( 11 ) < 8 ? ((1 << ( 11 )) << 8) : ((1 << ( 11 )) >> 8)) 	 
};


 










extern __const unsigned short int *__ctype_b;	 
extern __const __int32_t *__ctype_tolower;  
extern __const __int32_t *__ctype_toupper;  









 



extern int  isalnum   (int)     ;
extern int  isalpha   (int)     ;
extern int  iscntrl   (int)     ;
extern int  isdigit   (int)     ;
extern int  islower   (int)     ;
extern int  isgraph   (int)     ;
extern int  isprint   (int)     ;
extern int  ispunct   (int)     ;
extern int  isspace   (int)     ;
extern int  isupper   (int)     ;
extern int  isxdigit   (int)     ;






 
extern int tolower  (int __c)    ;

 
extern int toupper  (int __c)    ;




 

extern int isascii  (int __c)    ;

 

extern int toascii  (int __c)    ;




 

extern int  _toupper   (int)     ;
extern int  _tolower   (int)     ;





















extern __inline int
tolower (int __c)  
{
  return __c >= -128 && __c < 256 ? __ctype_tolower[__c] : __c;
}

extern __inline int
toupper (int __c)  
{
  return __c >= -128 && __c < 256 ? __ctype_toupper[__c] : __c;
}




# 182 "/usr/include/ctype.h" 3
















# 272 "/usr/include/ctype.h" 3


 


# 3 "mm.head.h" 2

# 1 "/usr/include/math.h" 1 3
 


















 








 

 

# 1 "/usr/include/bits/huge_val.h" 1 3
 

























 













 

# 68 "/usr/include/bits/huge_val.h" 3

# 33 "/usr/include/math.h" 2 3


 



 
# 1 "/usr/include/bits/mathdef.h" 1 3
 





















# 45 "/usr/include/bits/mathdef.h" 3

# 40 "/usr/include/math.h" 2 3



 



















# 1 "/usr/include/bits/mathcalls.h" 1 3
 


















 






























 

 
extern   double          acos          (double  __x)    ; extern   double         __acos          (double  __x)      ;
 
extern   double          asin          (double  __x)    ; extern   double         __asin          (double  __x)      ;
 
extern   double          atan          (double  __x)    ; extern   double         __atan          (double  __x)      ;
 
extern   double          atan2          (double  __y, double  __x)    ; extern   double         __atan2          (double  __y, double  __x)      ;

 
extern   double          cos          (double  __x)    ; extern   double         __cos          (double  __x)      ;
 
extern   double          sin          (double  __x)    ; extern   double         __sin          (double  __x)      ;
 
extern   double          tan          (double  __x)    ; extern   double         __tan          (double  __x)      ;







 

 
extern   double          cosh          (double  __x)    ; extern   double         __cosh          (double  __x)      ;
 
extern   double          sinh          (double  __x)    ; extern   double         __sinh          (double  __x)      ;
 
extern   double          tanh          (double  __x)    ; extern   double         __tanh          (double  __x)      ;


 
extern   double          acosh          (double  __x)    ; extern   double         __acosh          (double  __x)      ;
 
extern   double          asinh          (double  __x)    ; extern   double         __asinh          (double  __x)      ;
 
extern   double          atanh          (double  __x)    ; extern   double         __atanh          (double  __x)      ;


 

 
extern   double          exp          (double  __x)    ; extern   double         __exp          (double  __x)      ;








 
extern   double          frexp          (double  __x, int *__exponent)    ; extern   double         __frexp          (double  __x, int *__exponent)      ;

 
extern   double          ldexp          (double  __x, int __exponent)    ; extern   double         __ldexp          (double  __x, int __exponent)      ;

 
extern   double          log          (double  __x)    ; extern   double         __log          (double  __x)      ;

 
extern   double          log10          (double  __x)    ; extern   double         __log10          (double  __x)      ;

 
extern   double          modf          (double  __x, double  *__iptr)    ; extern   double         __modf          (double  __x, double  *__iptr)      ;


 
extern   double          expm1          (double  __x)    ; extern   double         __expm1          (double  __x)      ;

 
extern   double          log1p          (double  __x)    ; extern   double         __log1p          (double  __x)      ;

 
extern   double          logb          (double  __x)    ; extern   double         __logb          (double  __x)      ;











 

 
extern   double          pow          (double  __x, double  __y)    ; extern   double         __pow          (double  __x, double  __y)      ;

 
extern   double          sqrt          (double  __x)    ; extern   double         __sqrt          (double  __x)      ;


 
extern   double          hypot          (double  __x, double  __y)    ; extern   double         __hypot          (double  __x, double  __y)      ;



 
extern   double          cbrt          (double  __x)    ; extern   double         __cbrt          (double  __x)      ;



 

 
extern   double          ceil          (double  __x)    ; extern   double         __ceil          (double  __x)      ;

 
extern   double          fabs          (double  __x)     __attribute__ (    (__const__)  ); extern   double         __fabs          (double  __x)     __attribute__ (    (__const__)  )  ;

 
extern   double          floor          (double  __x)    ; extern   double         __floor          (double  __x)      ;

 
extern   double          fmod          (double  __x, double  __y)    ; extern   double         __fmod          (double  __x, double  __y)      ;


 

extern  int     __isinf      (double  __value)   __attribute__ ((__const__));


 

extern  int     isinf      (double  __value)   __attribute__ ((__const__));

 
extern   int        finite        (double  __value)    __attribute__ (  (__const__) ); extern   int        __finite        (double  __value)    __attribute__ (  (__const__) ) ;

 





extern   double          infnan          (int __error)     __attribute__ (    (__const__)  ); extern   double         __infnan          (int __error)     __attribute__ (    (__const__)  )  ;

 
extern   double          drem          (double  __x, double  __y)    ; extern   double         __drem          (double  __x, double  __y)      ;


 
extern   double          significand          (double  __x)    ; extern   double         __significand          (double  __x)      ;



 
extern   double          copysign          (double  __x, double  __y)     __attribute__ (    (__const__)  ); extern   double         __copysign          (double  __x, double  __y)     __attribute__ (    (__const__)  )  ;









 
extern   int        isnan        (double  __value)    __attribute__ (  (__const__) ); extern   int        __isnan        (double  __value)    __attribute__ (  (__const__) ) ;

 
extern   double          j0          (double )    ; extern   double         __j0          (double )      ;
extern   double          j1          (double )    ; extern   double         __j1          (double )      ;
extern   double          jn          (int, double )    ; extern   double         __jn          (int, double )      ;
extern   double          y0          (double )    ; extern   double         __y0          (double )      ;
extern   double          y1          (double )    ; extern   double         __y1          (double )      ;
extern   double          yn          (int, double )    ; extern   double         __yn          (int, double )      ;




 
extern   double          erf          (double )    ; extern   double         __erf          (double )      ;
extern   double          erfc          (double )    ; extern   double         __erfc          (double )      ;
extern   double          lgamma          (double )    ; extern   double         __lgamma          (double )      ;
extern   double          tgamma          (double )    ; extern   double         __tgamma          (double )      ;



 
extern   double          gamma          (double )    ; extern   double         __gamma          (double )      ;



 


extern   double          lgamma_r              (double , int *__signgamp)    ; extern   double         __lgamma_r              (double , int *__signgamp)      ;




 

extern   double          rint          (double  __x)    ; extern   double         __rint          (double  __x)      ;

 
extern   double          nextafter          (double  __x, double  __y)     __attribute__ (    (__const__)  ); extern   double         __nextafter          (double  __x, double  __y)     __attribute__ (    (__const__)  )  ;




 
extern   double          remainder          (double  __x, double  __y)    ; extern   double         __remainder          (double  __x, double  __y)      ;


 
extern   double          scalb          (double  __x, double  __n)    ; extern   double         __scalb          (double  __x, double  __n)      ;


 
extern   double          scalbn          (double  __x, int __n)    ; extern   double         __scalbn          (double  __x, int __n)      ;

 
extern   int        ilogb        (double  __x)   ; extern   int        __ilogb        (double  __x)    ;


# 330 "/usr/include/bits/mathcalls.h" 3

# 63 "/usr/include/math.h" 2 3







 











# 1 "/usr/include/bits/mathcalls.h" 1 3
 


















 






























 

 
extern   float          acosf         (float   __x)    ; extern   float         __acosf         (float   __x)      ;
 
extern   float          asinf         (float   __x)    ; extern   float         __asinf         (float   __x)      ;
 
extern   float          atanf         (float   __x)    ; extern   float         __atanf         (float   __x)      ;
 
extern   float          atan2f         (float   __y, float   __x)    ; extern   float         __atan2f         (float   __y, float   __x)      ;

 
extern   float          cosf         (float   __x)    ; extern   float         __cosf         (float   __x)      ;
 
extern   float          sinf         (float   __x)    ; extern   float         __sinf         (float   __x)      ;
 
extern   float          tanf         (float   __x)    ; extern   float         __tanf         (float   __x)      ;







 

 
extern   float          coshf         (float   __x)    ; extern   float         __coshf         (float   __x)      ;
 
extern   float          sinhf         (float   __x)    ; extern   float         __sinhf         (float   __x)      ;
 
extern   float          tanhf         (float   __x)    ; extern   float         __tanhf         (float   __x)      ;


 
extern   float          acoshf         (float   __x)    ; extern   float         __acoshf         (float   __x)      ;
 
extern   float          asinhf         (float   __x)    ; extern   float         __asinhf         (float   __x)      ;
 
extern   float          atanhf         (float   __x)    ; extern   float         __atanhf         (float   __x)      ;


 

 
extern   float          expf         (float   __x)    ; extern   float         __expf         (float   __x)      ;








 
extern   float          frexpf         (float   __x, int *__exponent)    ; extern   float         __frexpf         (float   __x, int *__exponent)      ;

 
extern   float          ldexpf         (float   __x, int __exponent)    ; extern   float         __ldexpf         (float   __x, int __exponent)      ;

 
extern   float          logf         (float   __x)    ; extern   float         __logf         (float   __x)      ;

 
extern   float          log10f         (float   __x)    ; extern   float         __log10f         (float   __x)      ;

 
extern   float          modff         (float   __x, float   *__iptr)    ; extern   float         __modff         (float   __x, float   *__iptr)      ;


 
extern   float          expm1f         (float   __x)    ; extern   float         __expm1f         (float   __x)      ;

 
extern   float          log1pf         (float   __x)    ; extern   float         __log1pf         (float   __x)      ;

 
extern   float          logbf         (float   __x)    ; extern   float         __logbf         (float   __x)      ;











 

 
extern   float          powf         (float   __x, float   __y)    ; extern   float         __powf         (float   __x, float   __y)      ;

 
extern   float          sqrtf         (float   __x)    ; extern   float         __sqrtf         (float   __x)      ;


 
extern   float          hypotf         (float   __x, float   __y)    ; extern   float         __hypotf         (float   __x, float   __y)      ;



 
extern   float          cbrtf         (float   __x)    ; extern   float         __cbrtf         (float   __x)      ;



 

 
extern   float          ceilf         (float   __x)    ; extern   float         __ceilf         (float   __x)      ;

 
extern   float          fabsf         (float   __x)     __attribute__ (    (__const__)  ); extern   float         __fabsf         (float   __x)     __attribute__ (    (__const__)  )  ;

 
extern   float          floorf         (float   __x)    ; extern   float         __floorf         (float   __x)      ;

 
extern   float          fmodf         (float   __x, float   __y)    ; extern   float         __fmodf         (float   __x, float   __y)      ;


 

extern  int    __isinff     (float   __value)   __attribute__ ((__const__));


 

extern  int    isinff     (float   __value)   __attribute__ ((__const__));

 
extern   int       finitef       (float   __value)    __attribute__ (  (__const__) ); extern   int       __finitef       (float   __value)    __attribute__ (  (__const__) ) ;

 





extern   float          infnanf         (int __error)     __attribute__ (    (__const__)  ); extern   float         __infnanf         (int __error)     __attribute__ (    (__const__)  )  ;

 
extern   float          dremf         (float   __x, float   __y)    ; extern   float         __dremf         (float   __x, float   __y)      ;


 
extern   float          significandf         (float   __x)    ; extern   float         __significandf         (float   __x)      ;



 
extern   float          copysignf         (float   __x, float   __y)     __attribute__ (    (__const__)  ); extern   float         __copysignf         (float   __x, float   __y)     __attribute__ (    (__const__)  )  ;









 
extern   int       isnanf       (float   __value)    __attribute__ (  (__const__) ); extern   int       __isnanf       (float   __value)    __attribute__ (  (__const__) ) ;

 
extern   float          j0f         (float  )    ; extern   float         __j0f         (float  )      ;
extern   float          j1f         (float  )    ; extern   float         __j1f         (float  )      ;
extern   float          jnf         (int, float  )    ; extern   float         __jnf         (int, float  )      ;
extern   float          y0f         (float  )    ; extern   float         __y0f         (float  )      ;
extern   float          y1f         (float  )    ; extern   float         __y1f         (float  )      ;
extern   float          ynf         (int, float  )    ; extern   float         __ynf         (int, float  )      ;




 
extern   float          erff         (float  )    ; extern   float         __erff         (float  )      ;
extern   float          erfcf         (float  )    ; extern   float         __erfcf         (float  )      ;
extern   float          lgammaf         (float  )    ; extern   float         __lgammaf         (float  )      ;
extern   float          tgammaf         (float  )    ; extern   float         __tgammaf         (float  )      ;



 
extern   float          gammaf         (float  )    ; extern   float         __gammaf         (float  )      ;



 


extern   float          lgammaf_r            (float  , int *__signgamp)    ; extern   float         __lgammaf_r            (float  , int *__signgamp)      ;




 

extern   float          rintf         (float   __x)    ; extern   float         __rintf         (float   __x)      ;

 
extern   float          nextafterf         (float   __x, float   __y)     __attribute__ (    (__const__)  ); extern   float         __nextafterf         (float   __x, float   __y)     __attribute__ (    (__const__)  )  ;




 
extern   float          remainderf         (float   __x, float   __y)    ; extern   float         __remainderf         (float   __x, float   __y)      ;


 
extern   float          scalbf         (float   __x, float   __n)    ; extern   float         __scalbf         (float   __x, float   __n)      ;


 
extern   float          scalbnf         (float   __x, int __n)    ; extern   float         __scalbnf         (float   __x, int __n)      ;

 
extern   int       ilogbf       (float   __x)   ; extern   int       __ilogbf       (float   __x)    ;


# 330 "/usr/include/bits/mathcalls.h" 3

# 82 "/usr/include/math.h" 2 3





 











# 1 "/usr/include/bits/mathcalls.h" 1 3
 


















 






























 

 
extern   long double          acosl         (long double   __x)    ; extern   long double         __acosl         (long double   __x)      ;
 
extern   long double          asinl         (long double   __x)    ; extern   long double         __asinl         (long double   __x)      ;
 
extern   long double          atanl         (long double   __x)    ; extern   long double         __atanl         (long double   __x)      ;
 
extern   long double          atan2l         (long double   __y, long double   __x)    ; extern   long double         __atan2l         (long double   __y, long double   __x)      ;

 
extern   long double          cosl         (long double   __x)    ; extern   long double         __cosl         (long double   __x)      ;
 
extern   long double          sinl         (long double   __x)    ; extern   long double         __sinl         (long double   __x)      ;
 
extern   long double          tanl         (long double   __x)    ; extern   long double         __tanl         (long double   __x)      ;







 

 
extern   long double          coshl         (long double   __x)    ; extern   long double         __coshl         (long double   __x)      ;
 
extern   long double          sinhl         (long double   __x)    ; extern   long double         __sinhl         (long double   __x)      ;
 
extern   long double          tanhl         (long double   __x)    ; extern   long double         __tanhl         (long double   __x)      ;


 
extern   long double          acoshl         (long double   __x)    ; extern   long double         __acoshl         (long double   __x)      ;
 
extern   long double          asinhl         (long double   __x)    ; extern   long double         __asinhl         (long double   __x)      ;
 
extern   long double          atanhl         (long double   __x)    ; extern   long double         __atanhl         (long double   __x)      ;


 

 
extern   long double          expl         (long double   __x)    ; extern   long double         __expl         (long double   __x)      ;








 
extern   long double          frexpl         (long double   __x, int *__exponent)    ; extern   long double         __frexpl         (long double   __x, int *__exponent)      ;

 
extern   long double          ldexpl         (long double   __x, int __exponent)    ; extern   long double         __ldexpl         (long double   __x, int __exponent)      ;

 
extern   long double          logl         (long double   __x)    ; extern   long double         __logl         (long double   __x)      ;

 
extern   long double          log10l         (long double   __x)    ; extern   long double         __log10l         (long double   __x)      ;

 
extern   long double          modfl         (long double   __x, long double   *__iptr)    ; extern   long double         __modfl         (long double   __x, long double   *__iptr)      ;


 
extern   long double          expm1l         (long double   __x)    ; extern   long double         __expm1l         (long double   __x)      ;

 
extern   long double          log1pl         (long double   __x)    ; extern   long double         __log1pl         (long double   __x)      ;

 
extern   long double          logbl         (long double   __x)    ; extern   long double         __logbl         (long double   __x)      ;











 

 
extern   long double          powl         (long double   __x, long double   __y)    ; extern   long double         __powl         (long double   __x, long double   __y)      ;

 
extern   long double          sqrtl         (long double   __x)    ; extern   long double         __sqrtl         (long double   __x)      ;


 
extern   long double          hypotl         (long double   __x, long double   __y)    ; extern   long double         __hypotl         (long double   __x, long double   __y)      ;



 
extern   long double          cbrtl         (long double   __x)    ; extern   long double         __cbrtl         (long double   __x)      ;



 

 
extern   long double          ceill         (long double   __x)    ; extern   long double         __ceill         (long double   __x)      ;

 
extern   long double          fabsl         (long double   __x)     __attribute__ (    (__const__)  ); extern   long double         __fabsl         (long double   __x)     __attribute__ (    (__const__)  )  ;

 
extern   long double          floorl         (long double   __x)    ; extern   long double         __floorl         (long double   __x)      ;

 
extern   long double          fmodl         (long double   __x, long double   __y)    ; extern   long double         __fmodl         (long double   __x, long double   __y)      ;


 

extern  int    __isinfl     (long double   __value)   __attribute__ ((__const__));


 

extern  int    isinfl     (long double   __value)   __attribute__ ((__const__));

 
extern   int       finitel       (long double   __value)    __attribute__ (  (__const__) ); extern   int       __finitel       (long double   __value)    __attribute__ (  (__const__) ) ;

 





extern   long double          infnanl         (int __error)     __attribute__ (    (__const__)  ); extern   long double         __infnanl         (int __error)     __attribute__ (    (__const__)  )  ;

 
extern   long double          dreml         (long double   __x, long double   __y)    ; extern   long double         __dreml         (long double   __x, long double   __y)      ;


 
extern   long double          significandl         (long double   __x)    ; extern   long double         __significandl         (long double   __x)      ;



 
extern   long double          copysignl         (long double   __x, long double   __y)     __attribute__ (    (__const__)  ); extern   long double         __copysignl         (long double   __x, long double   __y)     __attribute__ (    (__const__)  )  ;









 
extern   int       isnanl       (long double   __value)    __attribute__ (  (__const__) ); extern   int       __isnanl       (long double   __value)    __attribute__ (  (__const__) ) ;

 
extern   long double          j0l         (long double  )    ; extern   long double         __j0l         (long double  )      ;
extern   long double          j1l         (long double  )    ; extern   long double         __j1l         (long double  )      ;
extern   long double          jnl         (int, long double  )    ; extern   long double         __jnl         (int, long double  )      ;
extern   long double          y0l         (long double  )    ; extern   long double         __y0l         (long double  )      ;
extern   long double          y1l         (long double  )    ; extern   long double         __y1l         (long double  )      ;
extern   long double          ynl         (int, long double  )    ; extern   long double         __ynl         (int, long double  )      ;




 
extern   long double          erfl         (long double  )    ; extern   long double         __erfl         (long double  )      ;
extern   long double          erfcl         (long double  )    ; extern   long double         __erfcl         (long double  )      ;
extern   long double          lgammal         (long double  )    ; extern   long double         __lgammal         (long double  )      ;
extern   long double          tgammal         (long double  )    ; extern   long double         __tgammal         (long double  )      ;



 
extern   long double          gammal         (long double  )    ; extern   long double         __gammal         (long double  )      ;



 


extern   long double          lgammal_r            (long double  , int *__signgamp)    ; extern   long double         __lgammal_r            (long double  , int *__signgamp)      ;




 

extern   long double          rintl         (long double   __x)    ; extern   long double         __rintl         (long double   __x)      ;

 
extern   long double          nextafterl         (long double   __x, long double   __y)     __attribute__ (    (__const__)  ); extern   long double         __nextafterl         (long double   __x, long double   __y)     __attribute__ (    (__const__)  )  ;




 
extern   long double          remainderl         (long double   __x, long double   __y)    ; extern   long double         __remainderl         (long double   __x, long double   __y)      ;


 
extern   long double          scalbl         (long double   __x, long double   __n)    ; extern   long double         __scalbl         (long double   __x, long double   __n)      ;


 
extern   long double          scalbnl         (long double   __x, int __n)    ; extern   long double         __scalbnl         (long double   __x, int __n)      ;

 
extern   int       ilogbl       (long double   __x)   ; extern   int       __ilogbl       (long double   __x)    ;


# 330 "/usr/include/bits/mathcalls.h" 3

# 99 "/usr/include/math.h" 2 3













 
extern int signgam;



 
# 232 "/usr/include/math.h" 3



 
typedef enum
{
  _IEEE_ = -1,	 
  _SVID_,	 
  _XOPEN_,	 
  _POSIX_,
  _ISOC_	 
} _LIB_VERSION_TYPE;

 


extern _LIB_VERSION_TYPE _LIB_VERSION;




 







struct exception

  {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
  };




extern int matherr  (struct exception *__exc)    ;




 







 

# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h" 1 3
 



 

    


    


    


    


    


    


    


    


    


    


    



    


    


    


    


    


    


    


    


    



    


    


    



union __convert_long_double {
  unsigned __convert_long_double_i[4];
  long double __convert_long_double_d;
};


    


    


    


    


    


    




# 289 "/usr/include/math.h" 2 3


# 299 "/usr/include/math.h" 3



 
















 


# 336 "/usr/include/math.h" 3



 






 

# 1 "/usr/include/bits/mathinline.h" 1 3
 






























# 130 "/usr/include/bits/mathinline.h" 3



 






 





















































































 

extern __inline   double     __sgn   ( double );	extern __inline   double     __sgn   ( double      __x  )	{	      return __x == 0.0 ? 0.0 : (__x > 0.0 ? 1.0 : -1.0)  ;	} 	extern __inline   float      __sgnf    ( float );	extern __inline   float      __sgnf    ( float      __x  )	{	      return __x == 0.0 ? 0.0 : (__x > 0.0 ? 1.0 : -1.0)  ;	} 	extern __inline   long double      __sgnl    ( long double );	extern __inline   long double      __sgnl    ( long double      __x  )	{	      return __x == 0.0 ? 0.0 : (__x > 0.0 ? 1.0 : -1.0)  ;	}  


extern __inline   double     __pow2   ( double );	extern __inline   double     __pow2   ( double      __x  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __x;		        if (__x == (long double) __p)						          {									            __asm __volatile__						      	("fscale"							      	 : "=t" (__value) : "0" (1.0), "u" (__x));			            return __value;							          }									        __asm __volatile__							          ("fld	%%st(0)\n\t"						           "frndint			# int(x)\n\t"				           "fxch\n\t"								           "fsub	%%st(1)		# fract(x)\n\t"				           "f2xm1			# 2^(fract(x)) - 1\n\t"			           : "=t" (__value), "=u" (__exponent) : "0" (__x));			        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	} 	extern __inline   float      __pow2f    ( float );	extern __inline   float      __pow2f    ( float      __x  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __x;		        if (__x == (long double) __p)						          {									            __asm __volatile__						      	("fscale"							      	 : "=t" (__value) : "0" (1.0), "u" (__x));			            return __value;							          }									        __asm __volatile__							          ("fld	%%st(0)\n\t"						           "frndint			# int(x)\n\t"				           "fxch\n\t"								           "fsub	%%st(1)		# fract(x)\n\t"				           "f2xm1			# 2^(fract(x)) - 1\n\t"			           : "=t" (__value), "=u" (__exponent) : "0" (__x));			        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	} 	extern __inline   long double      __pow2l    ( long double );	extern __inline   long double      __pow2l    ( long double      __x  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __x;		        if (__x == (long double) __p)						          {									            __asm __volatile__						      	("fscale"							      	 : "=t" (__value) : "0" (1.0), "u" (__x));			            return __value;							          }									        __asm __volatile__							          ("fld	%%st(0)\n\t"						           "frndint			# int(x)\n\t"				           "fxch\n\t"								           "fsub	%%st(1)		# fract(x)\n\t"				           "f2xm1			# 2^(fract(x)) - 1\n\t"			           : "=t" (__value), "=u" (__exponent) : "0" (__x));			        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	}  
# 254 "/usr/include/bits/mathinline.h" 3


# 276 "/usr/include/bits/mathinline.h" 3

extern __inline  void __sincos (double __x, double *__sinx, double *__cosx);
extern __inline  void
__sincos (double __x, double *__sinx, double *__cosx)
{
  register long double __cosr;	register long double __sinr;	__asm __volatile__	("fsincos\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jz	1f\n\t"	"fldpi\n\t"	"fadd	%%st(0)\n\t"	"fxch	%%st(1)\n\t"	"2: fprem1\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jnz	2b\n\t"	"fstp	%%st(1)\n\t"	"fsincos\n\t"	"1:"	: "=t" (__cosr), "=u" (__sinr) : "0" (__x));	*__sinx = __sinr;	*__cosx = __cosr ;
}

extern __inline  void __sincosf (float __x, float *__sinx, float *__cosx);
extern __inline  void
__sincosf (float __x, float *__sinx, float *__cosx)
{
  register long double __cosr;	register long double __sinr;	__asm __volatile__	("fsincos\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jz	1f\n\t"	"fldpi\n\t"	"fadd	%%st(0)\n\t"	"fxch	%%st(1)\n\t"	"2: fprem1\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jnz	2b\n\t"	"fstp	%%st(1)\n\t"	"fsincos\n\t"	"1:"	: "=t" (__cosr), "=u" (__sinr) : "0" (__x));	*__sinx = __sinr;	*__cosx = __cosr ;
}

extern __inline  void __sincosl (long double __x, long double *__sinx,
			      long double *__cosx);
extern __inline  void
__sincosl (long double __x, long double *__sinx, long double *__cosx)
{
  register long double __cosr;	register long double __sinr;	__asm __volatile__	("fsincos\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jz	1f\n\t"	"fldpi\n\t"	"fadd	%%st(0)\n\t"	"fxch	%%st(1)\n\t"	"2: fprem1\n\t"	"fnstsw	%%ax\n\t"	"testl	$0x400, %%eax\n\t"	"jnz	2b\n\t"	"fstp	%%st(1)\n\t"	"fsincos\n\t"	"1:"	: "=t" (__cosr), "=u" (__sinr) : "0" (__x));	*__sinx = __sinr;	*__cosx = __cosr ;
}


 



# 322 "/usr/include/bits/mathinline.h" 3
extern __inline   long double    __expm1l  ( long double );	extern __inline   long double    __expm1l  ( long double    __x )	{	  register long double __value;	register long double __exponent;	register long double __temp;	__asm __volatile__	("fldl2e			# e^x - 1 = 2^(x * log2(e)) - 1\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	"fscale			# 2^(x * log2(e)) - 2^(int(x * log2(e)))\n\t" : "=t" (__value), "=u" (__exponent) : "0" (__x));	__asm __volatile__	("fscale			# 2^int(x * log2(e))\n\t"	: "=t" (__temp) : "0" (1.0), "u" (__exponent));	__temp -= 1.0;	return __temp + __value  ;	} 



# 342 "/usr/include/bits/mathinline.h" 3
extern __inline   double     exp   ( double );	extern __inline   double     exp   ( double      __x  )	{	    register long double __value;	register long double __exponent;	__asm __volatile__	("fldl2e			# e^x = 2^(x * log2(e))\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	: "=t" (__value), "=u" (__exponent) : "0" (__x));	__value += 1.0;	__asm __volatile__	("fscale"	: "=t" (__value) : "0" (__value), "u" (__exponent));	return __value   ;	} 	extern __inline   float      expf    ( float );	extern __inline   float      expf    ( float      __x  )	{	    register long double __value;	register long double __exponent;	__asm __volatile__	("fldl2e			# e^x = 2^(x * log2(e))\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	: "=t" (__value), "=u" (__exponent) : "0" (__x));	__value += 1.0;	__asm __volatile__	("fscale"	: "=t" (__value) : "0" (__value), "u" (__exponent));	return __value   ;	} 	extern __inline   long double      expl    ( long double );	extern __inline   long double      expl    ( long double      __x  )	{	    register long double __value;	register long double __exponent;	__asm __volatile__	("fldl2e			# e^x = 2^(x * log2(e))\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	: "=t" (__value), "=u" (__exponent) : "0" (__x));	__value += 1.0;	__asm __volatile__	("fscale"	: "=t" (__value) : "0" (__value), "u" (__exponent));	return __value   ;	}  
extern __inline   long double    __expl  ( long double );	extern __inline   long double    __expl  ( long double    __x )	{	  register long double __value;	register long double __exponent;	__asm __volatile__	("fldl2e			# e^x = 2^(x * log2(e))\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	: "=t" (__value), "=u" (__exponent) : "0" (__x));	__value += 1.0;	__asm __volatile__	("fscale"	: "=t" (__value) : "0" (__value), "u" (__exponent));	return __value  ;	} 


extern __inline   double     tan   ( double );	extern __inline   double     tan   ( double      __x  )	{	      register long double __value;						        register long double __value2 __attribute__ ((__unused__));		        __asm __volatile__							          ("fptan"								           : "=t" (__value2), "=u" (__value) : "0" (__x));			        return __value  ;	} 	extern __inline   float      tanf    ( float );	extern __inline   float      tanf    ( float      __x  )	{	      register long double __value;						        register long double __value2 __attribute__ ((__unused__));		        __asm __volatile__							          ("fptan"								           : "=t" (__value2), "=u" (__value) : "0" (__x));			        return __value  ;	} 	extern __inline   long double      tanl    ( long double );	extern __inline   long double      tanl    ( long double      __x  )	{	      register long double __value;						        register long double __value2 __attribute__ ((__unused__));		        __asm __volatile__							          ("fptan"								           : "=t" (__value2), "=u" (__value) : "0" (__x));			        return __value  ;	}  














extern __inline   double     atan2   ( double ,  double );	extern __inline   double     atan2   ( double      __y  ,  double      __x  )	{	    register long double __value;	__asm __volatile__	("fpatan\n\t"	: "=t" (__value) : "0" (__x), "u" (__y) : "st(1)");	return __value   ;	} 	extern __inline   float      atan2f    ( float ,  float );	extern __inline   float      atan2f    ( float      __y  ,  float      __x  )	{	    register long double __value;	__asm __volatile__	("fpatan\n\t"	: "=t" (__value) : "0" (__x), "u" (__y) : "st(1)");	return __value   ;	} 	extern __inline   long double      atan2l    ( long double ,  long double );	extern __inline   long double      atan2l    ( long double      __y  ,  long double      __x  )	{	    register long double __value;	__asm __volatile__	("fpatan\n\t"	: "=t" (__value) : "0" (__x), "u" (__y) : "st(1)");	return __value   ;	}  
extern __inline   long double    __atan2l  ( long double ,  long double );	extern __inline   long double    __atan2l  ( long double    __y ,  long double    __x )	{	  register long double __value;	__asm __volatile__	("fpatan\n\t"	: "=t" (__value) : "0" (__x), "u" (__y) : "st(1)");	return __value  ;	} 


extern __inline   double     fmod   ( double ,  double );	extern __inline   double     fmod   ( double      __x  ,  double      __y  )	{	      register long double __value;						        __asm __volatile__							          ("1:	fprem\n\t"						           "fnstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value) : "0" (__x), "u" (__y) : "ax", "cc");		        return __value  ;	} 	extern __inline   float      fmodf    ( float ,  float );	extern __inline   float      fmodf    ( float      __x  ,  float      __y  )	{	      register long double __value;						        __asm __volatile__							          ("1:	fprem\n\t"						           "fnstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value) : "0" (__x), "u" (__y) : "ax", "cc");		        return __value  ;	} 	extern __inline   long double      fmodl    ( long double ,  long double );	extern __inline   long double      fmodl    ( long double      __x  ,  long double      __y  )	{	      register long double __value;						        __asm __volatile__							          ("1:	fprem\n\t"						           "fnstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value) : "0" (__x), "u" (__y) : "ax", "cc");		        return __value  ;	}  
# 374 "/usr/include/bits/mathinline.h" 3


extern __inline   double     pow   ( double ,  double );	extern __inline   double     pow   ( double      __x  ,  double      __y  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __y;		        if (__x == 0.0 && __y > 0.0)						          return 0.0;								        if (__y == (double) __p)						          {									            long double __r = 1.0;						            if (__p == 0)							      	return 1.0;							            if (__p < 0)							      	{								      	  __p = -__p;							      	  __x = 1.0 / __x;						      	}								            while (1)								      	{								      	  if (__p & 1)							      	    __r *= __x;							      	  __p >>= 1;							      	  if (__p == 0)							      	    return __r;							      	  __x *= __x;							      	}								             							          }									        __asm __volatile__							          ("fyl2x" : "=t" (__value) : "0" (__x), "u" (1.0) : "st(1)");	        __asm __volatile__							          ("fmul	%%st(1)		# y * log2(x)\n\t"			           "fst	%%st(1)\n\t"						           "frndint			# int(y * log2(x))\n\t"			           "fxch\n\t"								           "fsub	%%st(1)		# fract(y * log2(x))\n\t"		           "f2xm1			# 2^(fract(y * log2(x))) - 1\n\t"	           : "=t" (__value), "=u" (__exponent) : "0" (__y), "1" (__value));	        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	} 	extern __inline   float      powf    ( float ,  float );	extern __inline   float      powf    ( float      __x  ,  float      __y  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __y;		        if (__x == 0.0 && __y > 0.0)						          return 0.0;								        if (__y == (double) __p)						          {									            long double __r = 1.0;						            if (__p == 0)							      	return 1.0;							            if (__p < 0)							      	{								      	  __p = -__p;							      	  __x = 1.0 / __x;						      	}								            while (1)								      	{								      	  if (__p & 1)							      	    __r *= __x;							      	  __p >>= 1;							      	  if (__p == 0)							      	    return __r;							      	  __x *= __x;							      	}								             							          }									        __asm __volatile__							          ("fyl2x" : "=t" (__value) : "0" (__x), "u" (1.0) : "st(1)");	        __asm __volatile__							          ("fmul	%%st(1)		# y * log2(x)\n\t"			           "fst	%%st(1)\n\t"						           "frndint			# int(y * log2(x))\n\t"			           "fxch\n\t"								           "fsub	%%st(1)		# fract(y * log2(x))\n\t"		           "f2xm1			# 2^(fract(y * log2(x))) - 1\n\t"	           : "=t" (__value), "=u" (__exponent) : "0" (__y), "1" (__value));	        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	} 	extern __inline   long double      powl    ( long double ,  long double );	extern __inline   long double      powl    ( long double      __x  ,  long double      __y  )	{	      register long double __value;						        register long double __exponent;					        __extension__ long long int __p = (long long int) __y;		        if (__x == 0.0 && __y > 0.0)						          return 0.0;								        if (__y == (double) __p)						          {									            long double __r = 1.0;						            if (__p == 0)							      	return 1.0;							            if (__p < 0)							      	{								      	  __p = -__p;							      	  __x = 1.0 / __x;						      	}								            while (1)								      	{								      	  if (__p & 1)							      	    __r *= __x;							      	  __p >>= 1;							      	  if (__p == 0)							      	    return __r;							      	  __x *= __x;							      	}								             							          }									        __asm __volatile__							          ("fyl2x" : "=t" (__value) : "0" (__x), "u" (1.0) : "st(1)");	        __asm __volatile__							          ("fmul	%%st(1)		# y * log2(x)\n\t"			           "fst	%%st(1)\n\t"						           "frndint			# int(y * log2(x))\n\t"			           "fxch\n\t"								           "fsub	%%st(1)		# fract(y * log2(x))\n\t"		           "f2xm1			# 2^(fract(y * log2(x))) - 1\n\t"	           : "=t" (__value), "=u" (__exponent) : "0" (__y), "1" (__value));	        __value += 1.0;							        __asm __volatile__							          ("fscale"								           : "=t" (__value) : "0" (__value), "u" (__exponent));		        return __value  ;	}  
# 418 "/usr/include/bits/mathinline.h" 3


extern __inline    double        sqrt    (  double  );	extern __inline    double        sqrt    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fsqrt"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         sqrtf     (  float  );	extern __inline    float         sqrtf     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fsqrt"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         sqrtl     (  long double  );	extern __inline    long double         sqrtl     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fsqrt"    : "=t" (__result) :   "0" (__x) );	return __result;	}   
extern __inline    long double       __sqrtl   (  long double  );	extern __inline    long double       __sqrtl   (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (    "fsqrt"   : "=t" (__result) :   "0" (__x) );	return __result;	}  


extern __inline   double    fabs  ( double );	extern __inline   double    fabs  ( double    __x )	{	  return __builtin_fabs (__x) ;	} 
extern __inline   float    fabsf  ( float );	extern __inline   float    fabsf  ( float    __x )	{	  return __builtin_fabsf (__x) ;	} 
extern __inline   long double    fabsl  ( long double );	extern __inline   long double    fabsl  ( long double    __x )	{	  return __builtin_fabsl (__x) ;	} 
extern __inline   long double    __fabsl  ( long double );	extern __inline   long double    __fabsl  ( long double    __x )	{	  return __builtin_fabsl (__x) ;	} 





 
extern __inline    double        sin    (  double  );	extern __inline    double        sin    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fsin"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         sinf     (  float  );	extern __inline    float         sinf     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fsin"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         sinl     (  long double  );	extern __inline    long double         sinl     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fsin"    : "=t" (__result) :   "0" (__x) );	return __result;	}   
 
extern __inline    double        cos    (  double  );	extern __inline    double        cos    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fcos"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         cosf     (  float  );	extern __inline    float         cosf     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fcos"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         cosl     (  long double  );	extern __inline    long double         cosl     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fcos"    : "=t" (__result) :   "0" (__x) );	return __result;	}   

extern __inline    double        atan    (  double  );	extern __inline    double        atan    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fld1; fpatan"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         atanf     (  float  );	extern __inline    float         atanf     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fld1; fpatan"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         atanl     (  long double  );	extern __inline    long double         atanl     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fld1; fpatan"    : "=t" (__result) :   "0" (__x) );	return __result;	}   
extern __inline    double        log    (  double  );	extern __inline    double        log    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fldln2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         logf     (  float  );	extern __inline    float         logf     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fldln2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         logl     (  long double  );	extern __inline    long double         logl     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fldln2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}   
extern __inline    double        log10    (  double  );	extern __inline    double        log10    (  double   __x)	{	register   double   __result;	__asm __volatile__ (      "fldlg2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    float         log10f     (  float  );	extern __inline    float         log10f     (  float   __x)	{	register   float   __result;	__asm __volatile__ (      "fldlg2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}  	extern __inline    long double         log10l     (  long double  );	extern __inline    long double         log10l     (  long double   __x)	{	register   long double   __result;	__asm __volatile__ (      "fldlg2; fxch; fyl2x"    : "=t" (__result) :   "0" (__x) );	return __result;	}   

extern __inline   double     asin   ( double );	extern __inline   double     asin   ( double      __x  )	{	    return __atan2l (__x, __sqrtl (1.0 - __x * __x))  ;	} 	extern __inline   float      asinf    ( float );	extern __inline   float      asinf    ( float      __x  )	{	    return __atan2l (__x, __sqrtl (1.0 - __x * __x))  ;	} 	extern __inline   long double      asinl    ( long double );	extern __inline   long double      asinl    ( long double      __x  )	{	    return __atan2l (__x, __sqrtl (1.0 - __x * __x))  ;	}  
extern __inline   double     acos   ( double );	extern __inline   double     acos   ( double      __x  )	{	    return __atan2l (__sqrtl (1.0 - __x * __x), __x)  ;	} 	extern __inline   float      acosf    ( float );	extern __inline   float      acosf    ( float      __x  )	{	    return __atan2l (__sqrtl (1.0 - __x * __x), __x)  ;	} 	extern __inline   long double      acosl    ( long double );	extern __inline   long double      acosl    ( long double      __x  )	{	    return __atan2l (__sqrtl (1.0 - __x * __x), __x)  ;	}  

extern __inline   long double    __sgn1l  ( long double );	extern __inline   long double    __sgn1l  ( long double    __x )	{	  return __x >= 0.0 ? 1.0 : -1.0 ;	} 


 
extern __inline   double     sinh   ( double );	extern __inline   double     sinh   ( double      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0) + __exm1) * __sgn1l (__x)  ;	} 	extern __inline   float      sinhf    ( float );	extern __inline   float      sinhf    ( float      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0) + __exm1) * __sgn1l (__x)  ;	} 	extern __inline   long double      sinhl    ( long double );	extern __inline   long double      sinhl    ( long double      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0) + __exm1) * __sgn1l (__x)  ;	}  



extern __inline   double     cosh   ( double );	extern __inline   double     cosh   ( double      __x  )	{	      register long double __ex = __expl (__x);				        return 0.5 * (__ex + 1.0 / __ex)  ;	} 	extern __inline   float      coshf    ( float );	extern __inline   float      coshf    ( float      __x  )	{	      register long double __ex = __expl (__x);				        return 0.5 * (__ex + 1.0 / __ex)  ;	} 	extern __inline   long double      coshl    ( long double );	extern __inline   long double      coshl    ( long double      __x  )	{	      register long double __ex = __expl (__x);				        return 0.5 * (__ex + 1.0 / __ex)  ;	}  



extern __inline   double     tanh   ( double );	extern __inline   double     tanh   ( double      __x  )	{	      register long double __exm1 = __expm1l (-__fabsl (__x + __x));	        return __exm1 / (__exm1 + 2.0) * __sgn1l (-__x)  ;	} 	extern __inline   float      tanhf    ( float );	extern __inline   float      tanhf    ( float      __x  )	{	      register long double __exm1 = __expm1l (-__fabsl (__x + __x));	        return __exm1 / (__exm1 + 2.0) * __sgn1l (-__x)  ;	} 	extern __inline   long double      tanhl    ( long double );	extern __inline   long double      tanhl    ( long double      __x  )	{	      register long double __exm1 = __expm1l (-__fabsl (__x + __x));	        return __exm1 / (__exm1 + 2.0) * __sgn1l (-__x)  ;	}  




extern __inline   double     floor   ( double );	extern __inline   double     floor   ( double      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0400;  		        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	} 	extern __inline   float      floorf    ( float );	extern __inline   float      floorf    ( float      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0400;  		        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	} 	extern __inline   long double      floorl    ( long double );	extern __inline   long double      floorl    ( long double      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0400;  		        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	}  
# 472 "/usr/include/bits/mathinline.h" 3

extern __inline   double     ceil   ( double );	extern __inline   double     ceil   ( double      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0800;  			        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	} 	extern __inline   float      ceilf    ( float );	extern __inline   float      ceilf    ( float      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0800;  			        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	} 	extern __inline   long double      ceill    ( long double );	extern __inline   long double      ceill    ( long double      __x  )	{	      register long double __value;						        __volatile unsigned short int __cw;					        __volatile unsigned short int __cwtmp;				        __asm __volatile ("fnstcw %0" : "=m" (__cw));				        __cwtmp = (__cw & 0xf3ff) | 0x0800;  			        __asm __volatile ("fldcw %0" : : "m" (__cwtmp));			        __asm __volatile ("frndint" : "=t" (__value) : "0" (__x));		        __asm __volatile ("fldcw %0" : : "m" (__cw));				        return __value  ;	}  
# 483 "/usr/include/bits/mathinline.h" 3








extern __inline  double ldexp (double __x, int __y);
extern __inline  double
ldexp (double __x, int __y)
{
  register long double __value;	__asm __volatile__	("fscale"	: "=t" (__value) : "0" (__x), "u" ((long double) __y));	return __value ;
}


 


extern __inline   double     expm1   ( double );	extern __inline   double     expm1   ( double      __x  )	{	    register long double __value;	register long double __exponent;	register long double __temp;	__asm __volatile__	("fldl2e			# e^x - 1 = 2^(x * log2(e)) - 1\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	"fscale			# 2^(x * log2(e)) - 2^(int(x * log2(e)))\n\t" : "=t" (__value), "=u" (__exponent) : "0" (__x));	__asm __volatile__	("fscale			# 2^int(x * log2(e))\n\t"	: "=t" (__temp) : "0" (1.0), "u" (__exponent));	__temp -= 1.0;	return __temp + __value   ;	} 	extern __inline   float      expm1f    ( float );	extern __inline   float      expm1f    ( float      __x  )	{	    register long double __value;	register long double __exponent;	register long double __temp;	__asm __volatile__	("fldl2e			# e^x - 1 = 2^(x * log2(e)) - 1\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	"fscale			# 2^(x * log2(e)) - 2^(int(x * log2(e)))\n\t" : "=t" (__value), "=u" (__exponent) : "0" (__x));	__asm __volatile__	("fscale			# 2^int(x * log2(e))\n\t"	: "=t" (__temp) : "0" (1.0), "u" (__exponent));	__temp -= 1.0;	return __temp + __value   ;	} 	extern __inline   long double      expm1l    ( long double );	extern __inline   long double      expm1l    ( long double      __x  )	{	    register long double __value;	register long double __exponent;	register long double __temp;	__asm __volatile__	("fldl2e			# e^x - 1 = 2^(x * log2(e)) - 1\n\t"	"fmul	%%st(1)		# x * log2(e)\n\t"	"fst	%%st(1)\n\t"	"frndint			# int(x * log2(e))\n\t"	"fxch\n\t"	"fsub	%%st(1)		# fract(x * log2(e))\n\t"	"f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"	"fscale			# 2^(x * log2(e)) - 2^(int(x * log2(e)))\n\t" : "=t" (__value), "=u" (__exponent) : "0" (__x));	__asm __volatile__	("fscale			# 2^int(x * log2(e))\n\t"	: "=t" (__temp) : "0" (1.0), "u" (__exponent));	__temp -= 1.0;	return __temp + __value   ;	}  

 



extern __inline   double     log1p   ( double );	extern __inline   double     log1p   ( double      __x  )	{	      register long double __value;						        if (__fabsl (__x) >= 1.0 - 0.5 * 1.41421356237309504880L )				          __value = logl (1.0 + __x);						        else									          __asm __volatile__							            ("fldln2\n\t"							             "fxch\n\t"							             "fyl2xp1"							             : "=t" (__value) : "0" (__x));					        return __value  ;	} 	extern __inline   float      log1pf    ( float );	extern __inline   float      log1pf    ( float      __x  )	{	      register long double __value;						        if (__fabsl (__x) >= 1.0 - 0.5 * 1.41421356237309504880L )				          __value = logl (1.0 + __x);						        else									          __asm __volatile__							            ("fldln2\n\t"							             "fxch\n\t"							             "fyl2xp1"							             : "=t" (__value) : "0" (__x));					        return __value  ;	} 	extern __inline   long double      log1pl    ( long double );	extern __inline   long double      log1pl    ( long double      __x  )	{	      register long double __value;						        if (__fabsl (__x) >= 1.0 - 0.5 * 1.41421356237309504880L )				          __value = logl (1.0 + __x);						        else									          __asm __volatile__							            ("fldln2\n\t"							             "fxch\n\t"							             "fyl2xp1"							             : "=t" (__value) : "0" (__x));					        return __value  ;	}  
# 519 "/usr/include/bits/mathinline.h" 3


 
extern __inline   double     asinh   ( double );	extern __inline   double     asinh   ( double      __x  )	{	      register long double  __y = __fabsl (__x);				        return (log1pl (__y * __y / (__sqrtl (__y * __y + 1.0) + 1.0) + __y)	      	  * __sgn1l (__x))  ;	} 	extern __inline   float      asinhf    ( float );	extern __inline   float      asinhf    ( float      __x  )	{	      register long double  __y = __fabsl (__x);				        return (log1pl (__y * __y / (__sqrtl (__y * __y + 1.0) + 1.0) + __y)	      	  * __sgn1l (__x))  ;	} 	extern __inline   long double      asinhl    ( long double );	extern __inline   long double      asinhl    ( long double      __x  )	{	      register long double  __y = __fabsl (__x);				        return (log1pl (__y * __y / (__sqrtl (__y * __y + 1.0) + 1.0) + __y)	      	  * __sgn1l (__x))  ;	}  




extern __inline   double     acosh   ( double );	extern __inline   double     acosh   ( double      __x  )	{	      return logl (__x + __sqrtl (__x - 1.0) * __sqrtl (__x + 1.0))  ;	} 	extern __inline   float      acoshf    ( float );	extern __inline   float      acoshf    ( float      __x  )	{	      return logl (__x + __sqrtl (__x - 1.0) * __sqrtl (__x + 1.0))  ;	} 	extern __inline   long double      acoshl    ( long double );	extern __inline   long double      acoshl    ( long double      __x  )	{	      return logl (__x + __sqrtl (__x - 1.0) * __sqrtl (__x + 1.0))  ;	}  


extern __inline   double     atanh   ( double );	extern __inline   double     atanh   ( double      __x  )	{	      register long double __y = __fabsl (__x);				        return -0.5 * log1pl (-(__y + __y) / (1.0 + __y)) * __sgn1l (__x)  ;	} 	extern __inline   float      atanhf    ( float );	extern __inline   float      atanhf    ( float      __x  )	{	      register long double __y = __fabsl (__x);				        return -0.5 * log1pl (-(__y + __y) / (1.0 + __y)) * __sgn1l (__x)  ;	} 	extern __inline   long double      atanhl    ( long double );	extern __inline   long double      atanhl    ( long double      __x  )	{	      register long double __y = __fabsl (__x);				        return -0.5 * log1pl (-(__y + __y) / (1.0 + __y)) * __sgn1l (__x)  ;	}  



 
extern __inline   double     hypot   ( double ,  double );	extern __inline   double     hypot   ( double      __x  ,  double      __y  )	{	    return __sqrtl (__x * __x + __y * __y)  ;	} 	extern __inline   float      hypotf    ( float ,  float );	extern __inline   float      hypotf    ( float      __x  ,  float      __y  )	{	    return __sqrtl (__x * __x + __y * __y)  ;	} 	extern __inline   long double      hypotl    ( long double ,  long double );	extern __inline   long double      hypotl    ( long double      __x  ,  long double      __y  )	{	    return __sqrtl (__x * __x + __y * __y)  ;	}  

extern __inline   double     logb   ( double );	extern __inline   double     logb   ( double      __x  )	{	      register long double __value;						        register long double __junk;						        __asm __volatile__							          ("fxtract\n\t"							           : "=t" (__junk), "=u" (__value) : "0" (__x));			        return __value  ;	} 	extern __inline   float      logbf    ( float );	extern __inline   float      logbf    ( float      __x  )	{	      register long double __value;						        register long double __junk;						        __asm __volatile__							          ("fxtract\n\t"							           : "=t" (__junk), "=u" (__value) : "0" (__x));			        return __value  ;	} 	extern __inline   long double      logbl    ( long double );	extern __inline   long double      logbl    ( long double      __x  )	{	      register long double __value;						        register long double __junk;						        __asm __volatile__							          ("fxtract\n\t"							           : "=t" (__junk), "=u" (__value) : "0" (__x));			        return __value  ;	}  









# 614 "/usr/include/bits/mathinline.h" 3





extern __inline   double     drem   ( double ,  double );	extern __inline   double     drem   ( double      __x  ,  double      __y  )	{	      register double __value;						        register int __clobbered;						        __asm __volatile__							          ("1:	fprem1\n\t"						           "fstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value), "=&a" (__clobbered) : "0" (__x), "u" (__y) : "cc");      return __value  ;	} 	extern __inline   float      dremf    ( float ,  float );	extern __inline   float      dremf    ( float      __x  ,  float      __y  )	{	      register double __value;						        register int __clobbered;						        __asm __volatile__							          ("1:	fprem1\n\t"						           "fstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value), "=&a" (__clobbered) : "0" (__x), "u" (__y) : "cc");      return __value  ;	} 	extern __inline   long double      dreml    ( long double ,  long double );	extern __inline   long double      dreml    ( long double      __x  ,  long double      __y  )	{	      register double __value;						        register int __clobbered;						        __asm __volatile__							          ("1:	fprem1\n\t"						           "fstsw	%%ax\n\t"						           "sahf\n\t"								           "jp	1b"							           : "=t" (__value), "=&a" (__clobbered) : "0" (__x), "u" (__y) : "cc");      return __value  ;	}  
# 629 "/usr/include/bits/mathinline.h" 3


 
extern __inline  int __finite (double __x) __attribute__ ((__const__));
extern __inline  int
__finite (double __x)
{
  return (__extension__
	  (((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
	     | 0x800fffff) + 1) >> 31));
}

 

extern __inline   double     __coshm1   ( double );	extern __inline   double     __coshm1   ( double      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0)) * __exm1  ;	} 	extern __inline   float      __coshm1f    ( float );	extern __inline   float      __coshm1f    ( float      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0)) * __exm1  ;	} 	extern __inline   long double      __coshm1l    ( long double );	extern __inline   long double      __coshm1l    ( long double      __x  )	{	      register long double __exm1 = __expm1l (__fabsl (__x));		        return 0.5 * (__exm1 / (__exm1 + 1.0)) * __exm1  ;	}  



extern __inline   double     __acosh1p   ( double );	extern __inline   double     __acosh1p   ( double      __x  )	{	      return log1pl (__x + __sqrtl (__x) * __sqrtl (__x + 2.0))  ;	} 	extern __inline   float      __acosh1pf    ( float );	extern __inline   float      __acosh1pf    ( float      __x  )	{	      return log1pl (__x + __sqrtl (__x) * __sqrtl (__x + 2.0))  ;	} 	extern __inline   long double      __acosh1pl    ( long double );	extern __inline   long double      __acosh1pl    ( long double      __x  )	{	      return log1pl (__x + __sqrtl (__x) * __sqrtl (__x + 2.0))  ;	}  




 








 
# 670 "/usr/include/bits/mathinline.h" 3



# 348 "/usr/include/math.h" 2 3




# 407 "/usr/include/math.h" 3


 



# 4 "mm.head.h" 2

# 1 "m.defs.h" 1
# 1 "m.structs.h" 1
 

struct ug_adesc {		 
    char a_type; 		 
    int  a_index; 		 
    struct ug_adesc *next_uarg;	 
};
struct ug_list {		 
    int (*arg_c)(); 		 
    int n_arg;         		 
    struct ug_adesc *arg_list; 	 
    struct ug_list *next_ug; 	 
};
struct ins_struct {		 
    char *ins_name; 		 
    int  Nb, bmax;  		 
    int  Np, pmax;  		 
    int  Nd, dmax;  		 
    int  Nf, fmax;  		 
    int  Nv, vmax;  		 
    int  Ns, smax;  		 
    int  Nn;			 
    int  Nc;        		 
    int  Nug;       		 
    float *n_list;		 
    struct ug_list *ugp; 	 
    struct ins_struct *next_ins; 
};
struct func_struct {		 
    int f_index;		 
    float *f_vals;		 
    float f_len;		 
    struct func_struct *next_fp; 
};
struct note_struct {		 
    char *n_ins;		 
    float n_start;		 
    float n_dur;		 
    float n_end;		 
    int n_flags;		 
    int nparams;		 
    float *plist;		 
    float *dlist;		 
    float *nlist;		 
    long *ug_flags;		 
    struct note_struct *next_note; 
};
# 1 "m.defs.h" 2


 


































































 




# 5 "mm.head.h" 2

# 1 "m.exts.h" 1
 

extern long Srate;	 
extern int Rflag;	 
extern long Nchan;	 
extern long Lblocks;	 
extern int Bflag;	 
extern long Lfuncs;	 
extern int Lflag;	 
extern long Nvars;	 
extern long Nparams;	 
extern long Verbose;	 
extern long Header;	 
extern int vflag;	 
extern long Notify;	 
extern int nflag;	 
extern long Timer;	 
extern int tflag;	 
extern long Floatout;	 
extern long Nsamp;	 
extern long Ngen;	 
extern long Nins;	 
extern long Clipped;	 
int Clip;		 
extern long Error;	 
extern long Sfbsize;	 
extern long Nooutput;	 
extern int  oflag;	 


 
extern float Lx[4];	 
extern float Ly[4];	 
extern int   NLs;	 
extern float Ax[4];	 
extern float Ay[4];	 
extern int   NAs;	 
extern float Sx[4];	 
extern float Sy[4];	 
extern float Revscale;	 
extern float T60mult;	 
extern float Revthresh;	 
extern float Direct;	 
extern float Reflect;	 
extern float *Grevblock; 
extern int Spaceon;	 
extern int Spacewason;	 
extern int Spacereset;	 
extern float Maxecho;	 
extern float Maxreverb;	 

extern float Time;	 
extern float First;	 
extern float Now;	 
extern float Future;	 
extern float Endtime;	 
extern float Maxamp;	 
extern float *Tempo;	 
extern float *Dtempo;	 
extern float *Offset;	 

extern FILE *Infile;	 
extern FILE *PInfile;	 
extern char *Infilename; 
extern int Mergeflag;	 
extern int Merging;	 
extern int Deferflag;	 
extern FILE *Outfile;	 
extern char *Outfilename;	 
extern FILE *Listfile;		 
extern char *Listfilename;	 
extern FILE *Barefile;		 
extern char *Barefilename;	 

extern float *Outblock;	 
extern float *Outptr;	 
extern short *Sampout;	 
extern long Lout;	 

extern float *Blocks;	 
extern float *Bp;	 
extern long Bmax;	 

extern float *Vars;	 
extern float *Vp;	 
extern long Vmax;	 

extern char **Strvars;	 
extern char **Strp;	 
extern long Strmax;	 

extern float *Params;	 
extern float *Pp;	 
extern long Pmax;	 

extern struct ins_struct *inslist;	 
extern struct ins_struct *ins;
extern struct ug_list *ug;
extern struct ug_adesc *uarg;

extern struct func_struct *fplist;	 
extern struct func_struct *fp;

extern struct note_struct *notelist;	 
extern struct note_struct *note;

extern struct ug_desc *up;		 

extern double Pi;		 
extern double Pi2;		 
extern char fld[1024], sfld[1024], ugn[1024], *sp;
extern char Junk[1024];
extern long bc;
float getp2();
float getp4();
# 6 "mm.head.h" 2

# 2 "mm.ins.c" 2

# 1 "ug.head.h" 1
# 1 "ug.defs.h" 1
 

# 1 "/usr/local/include/carl/sndio.h" 1 3
# 1 "/usr/local/include/whoami.h" 1 3




# 1 "/usr/local/include/carl/sndio.h" 2 3

# 1 "/usr/local/include/carl/defaults.h" 1 3
 

# 1 "/usr/local/include/whoami.h" 1 3




# 3 "/usr/local/include/carl/defaults.h" 2 3

# 1 "/usr/local/include/carl/paths.h" 1 3
 






# 4 "/usr/local/include/carl/defaults.h" 2 3




 













 







# 40 "/usr/local/include/carl/defaults.h" 3


 


















 




 

 


 

# 2 "/usr/local/include/carl/sndio.h" 2 3


 








 











 

 






 












 









 








 



 
 

 

 

 

 

 

 




 



 

 


 







 



struct dskblk {
    char flag;			 
    long base;			 
    long len;			 
    long seq;			 
    char *dfn;			 
    long cd;			 
    struct dskblk *next;
    struct dskblk *last;
    struct sndesc *dsksfd;	 
};


 



struct inclist {
    char *fn;
    struct inclist *incnext;
    struct inclist *inclast;
};

 



struct comlist {
    char *cmt;
    struct comlist *comnext;
    struct comlist *comlast;
};

struct csnd_prop {
	char *cprop_name;
	char *cprop_val;
	struct csnd_prop *cprop_next;
};

 






 


struct sndesc 
{
    struct sndesc *nxtsdf; 
    struct sndesc *lstsdf; 
    char 	*sfn;	 
    char	*sfown;	 
    char	rtflag;	 
    int		fprot;	 
    int		fhold;	 
    int		fid;	 
    int		spid;	 
    long	ncyls;	 
    struct dskblk *cp;	 




    char	*lkfile; 
    long	secptr;	 
    int		bufsiz;	 
			 
    int		blksiz;	 
			 
    int		sbptr;	 
    int		sbcnt;	 
    int		bdir;	 
    int		bfvb;	 
    short	*sb;	 
    float	*fb;	 
    int		ssize;	 
    int		nsib;	 
    int 	rw;	 
    int 	eof;	 
    int		err;	 
    float 	sr;	 
    char	pm;	 
    int		nc;	 
    long	fs;	 
    long	cdate;	 
    long	rdate;	 
    long	adate;	 
    long	dumpd;	 
    long	tpkey;	 
    struct comlist *comsdf;	 
    struct inclist *inclsdf;	 
    struct csnd_prop *csnd_p;	 
};





typedef struct sndesc CSNDFILE;

extern char crack();
extern char *arg_option;
extern int arg_index;
extern float sfexpr();
extern CSNDFILE *cpsfd();
float fsndi();
extern CSNDFILE *accesf();
extern CSNDFILE *opensf();
char *polish();
extern CSNDFILE *setsfd();
extern CSNDFILE *sopensf();
extern char *getsfile();
# 3 "ug.defs.h" 2

extern int sferror;








struct ug_desc{			 
    char *ug_name;		 
    char *ug_arglist;		 
    int  (*ug_call)();		 
};

union arg_ptr{  
    float *v; 
    float **fp; 
    double **dp; 
    float (*f)();
    char  *s;
    long  *l;
    FILE **fileptr;

    struct sndesc **sfd;




};










































# 1 "ug.head.h" 2

# 1 "ug.exts.h" 1
extern struct ug_desc ug_vec[];
# 2 "ug.head.h" 2

# 3 "mm.ins.c" 2


instrument(){
 int Bextend = 0, Vextend = 0, Pextend = 0, Sextend = 0;
 float *nptr;

    if(Barefile)fprintf(Barefile, "ins " ); ;
    field(fld,"\t\n" ," ,;\t" ); *(Params+2) = Time = getp2(fld);
    if(Time < Now){Error++;	sprintf(Junk,"X: Sequence error: @Time=%.3f < Now=%.3f\n",	Time,Now); print(Junk); if(bc!=';')while(field(fld,"\t\n" ," ,;\t" ) != ';') ;return; } ;
    if(Time > Now)synthesize();
    field(fld,"\t\n" ," ,;\t" );
    if(Barefile)fprintf(Barefile, "%s ;\n" , fld ); ;

    for( ins =  inslist ;  ins ;  ins = ins ->  next_ins ) 
     if(! __extension__	({ size_t __s1_len, __s2_len;	(__builtin_constant_p ( fld ) && __builtin_constant_p ( ins->ins_name )	&& (__s1_len = strlen ( fld ), __s2_len = strlen ( ins->ins_name ),	(! ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1)  || __s1_len >= 4)	&& (! ((size_t)(const void *)((  ins->ins_name  ) + 1) - (size_t)(const void *)(  ins->ins_name  ) == 1)  || __s2_len >= 4))	? memcmp ((__const char *) ( fld ), (__const char *) ( ins->ins_name ),	(__s1_len < __s2_len ? __s1_len : __s2_len) + 1)	: (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	&& (__s1_len = strlen ( fld ), __s1_len < 4)	? (__builtin_constant_p ( ins->ins_name ) && ((size_t)(const void *)((  ins->ins_name  ) + 1) - (size_t)(const void *)(  ins->ins_name  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   ins->ins_name  ))[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[1]);	if (  __s1_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[2]);	if (  __s1_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s2 =	(__const unsigned char *) (__const char *) (   ins->ins_name  );	register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - __s2[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1] - __s2[1]); if (  __s1_len  > 1 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[2] - __s2[2]);	if (  __s1_len  > 2 && __result == 0)	__result = (((__const unsigned char *)	(__const char *)  (  fld  ))[3]	- __s2[3]);	}	}	__result; })) )	: (__builtin_constant_p ( ins->ins_name ) && ((size_t)(const void *)((  ins->ins_name  ) + 1) - (size_t)(const void *)(  ins->ins_name  ) == 1) 	&& (__s2_len = strlen ( ins->ins_name ), __s2_len < 4)	? (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   ins->ins_name  ))[0]);	if (  __s2_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s1 =	(__const unsigned char *) (__const char *) (  fld  );	register int __result =	__s1[0] - ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[0];	if (  __s2_len  > 0 && __result == 0)	{	__result = (__s1[1]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(__s1[2] - ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(__s1[3]	- ((__const unsigned char *)	(__const char *) (   ins->ins_name  ))[3]);	}	}	__result; })) )	: strcmp ( fld ,  ins->ins_name )))); }) ){
      Error++;
      print("Instruments may not be redefined.\n"); {while(field(fld,"\t\n" ," ,;\t" ) != ';'); continue;} ;
     }
    if(!  inslist )  inslist  =  ins  = (struct    ins_struct   *) calloc(1, sizeof(struct    ins_struct  )) ; else{ for(  ins  =   inslist  ;   ins  ->   next_ins  ;   ins  =  ins  ->   next_ins  ) ;  ins ->  next_ins  = (struct    ins_struct   *) calloc(1, sizeof(struct    ins_struct  )) ;  ins  =  ins ->  next_ins ; } ;
    ins->ins_name = (__extension__ (__builtin_constant_p (  fld  )	? (((size_t)(const void *)((   fld   ) + 1) - (size_t)(const void *)(   fld   ) == 1)  && strlen (  fld  ) + 1 <= 8	? __strcpy_small ( malloc(strlen( fld )+1) , __extension__ (((__const unsigned char *) (__const char *) (    fld    ))[  0  + 1] << 8	| ((__const unsigned char *) (__const char *) (    fld    ))[  0 ]) ,	__extension__ (((__const unsigned char *) (__const char *) (    fld    ))[  4  + 1] << 8	| ((__const unsigned char *) (__const char *) (    fld    ))[  4 ]) ,	__extension__ (((((__const unsigned char *) (__const char *) (    fld    ))[  0  + 3] << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  0  + 2]) << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  0  + 1]) << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  0 ]) ,	__extension__ (((((__const unsigned char *) (__const char *) (    fld    ))[  4  + 3] << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  4  + 2]) << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  4  + 1]) << 8 | ((__const unsigned char *) (__const char *) (    fld    ))[  4 ])  ,	strlen (  fld  ) + 1)	: (char *) memcpy ( malloc(strlen( fld )+1) ,   fld  , strlen (  fld  ) + 1))	: strcpy ( malloc(strlen( fld )+1) ,   fld  )))  ;

    for(field(fld,"\t\n" ," ,;\t" ); __extension__	({ size_t __s1_len, __s2_len;	(__builtin_constant_p ( fld ) && __builtin_constant_p ( "end" )	&& (__s1_len = strlen ( fld ), __s2_len = strlen ( "end" ),	(! ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1)  || __s1_len >= 4)	&& (! ((size_t)(const void *)((  "end"  ) + 1) - (size_t)(const void *)(  "end"  ) == 1)  || __s2_len >= 4))	? memcmp ((__const char *) ( fld ), (__const char *) ( "end" ),	(__s1_len < __s2_len ? __s1_len : __s2_len) + 1)	: (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	&& (__s1_len = strlen ( fld ), __s1_len < 4)	? (__builtin_constant_p ( "end" ) && ((size_t)(const void *)((  "end"  ) + 1) - (size_t)(const void *)(  "end"  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   "end"  ))[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[1]);	if (  __s1_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[2]);	if (  __s1_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s2 =	(__const unsigned char *) (__const char *) (   "end"  );	register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - __s2[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1] - __s2[1]); if (  __s1_len  > 1 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[2] - __s2[2]);	if (  __s1_len  > 2 && __result == 0)	__result = (((__const unsigned char *)	(__const char *)  (  fld  ))[3]	- __s2[3]);	}	}	__result; })) )	: (__builtin_constant_p ( "end" ) && ((size_t)(const void *)((  "end"  ) + 1) - (size_t)(const void *)(  "end"  ) == 1) 	&& (__s2_len = strlen ( "end" ), __s2_len < 4)	? (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   "end"  ))[0]);	if (  __s2_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s1 =	(__const unsigned char *) (__const char *) (  fld  );	register int __result =	__s1[0] - ((__const unsigned char *)	(__const char *) (   "end"  ))[0];	if (  __s2_len  > 0 && __result == 0)	{	__result = (__s1[1]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(__s1[2] - ((__const unsigned char *)	(__const char *) (   "end"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(__s1[3]	- ((__const unsigned char *)	(__const char *) (   "end"  ))[3]);	}	}	__result; })) )	: strcmp ( fld ,  "end" )))); }) ; field(fld,"\t\n" ," ,;\t" )){
	if(Barefile)fprintf(Barefile, "%s " , fld ); ;
	if(!  ins->ugp )  ins->ugp  =  ug  = (struct    ug_list   *) calloc(1, sizeof(struct    ug_list  )) ; else{ for(  ug  =   ins->ugp  ;   ug  ->   next_ug  ;   ug  =  ug  ->   next_ug  ) ;  ug ->  next_ug  = (struct    ug_list   *) calloc(1, sizeof(struct    ug_list  )) ;  ug  =  ug ->  next_ug ; } ;
	ins->Nug++;

	for(up=ug_vec; up->ug_name; up++)
	 if(! __extension__	({ size_t __s1_len, __s2_len;	(__builtin_constant_p ( fld ) && __builtin_constant_p ( up->ug_name )	&& (__s1_len = strlen ( fld ), __s2_len = strlen ( up->ug_name ),	(! ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1)  || __s1_len >= 4)	&& (! ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1)  || __s2_len >= 4))	? memcmp ((__const char *) ( fld ), (__const char *) ( up->ug_name ),	(__s1_len < __s2_len ? __s1_len : __s2_len) + 1)	: (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	&& (__s1_len = strlen ( fld ), __s1_len < 4)	? (__builtin_constant_p ( up->ug_name ) && ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   up->ug_name  ))[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[1]);	if (  __s1_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[2]);	if (  __s1_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s2 =	(__const unsigned char *) (__const char *) (   up->ug_name  );	register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - __s2[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1] - __s2[1]); if (  __s1_len  > 1 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[2] - __s2[2]);	if (  __s1_len  > 2 && __result == 0)	__result = (((__const unsigned char *)	(__const char *)  (  fld  ))[3]	- __s2[3]);	}	}	__result; })) )	: (__builtin_constant_p ( up->ug_name ) && ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1) 	&& (__s2_len = strlen ( up->ug_name ), __s2_len < 4)	? (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   up->ug_name  ))[0]);	if (  __s2_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s1 =	(__const unsigned char *) (__const char *) (  fld  );	register int __result =	__s1[0] - ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[0];	if (  __s2_len  > 0 && __result == 0)	{	__result = (__s1[1]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(__s1[2] - ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(__s1[3]	- ((__const unsigned char *)	(__const char *) (   up->ug_name  ))[3]);	}	}	__result; })) )	: strcmp ( fld ,  up->ug_name )))); }) )break;
	if (!up->ug_name) {
		Error++; print(" Unrecognized") ;{ sprintf(Junk," Unit Generator '%s'", fld ); print(Junk); } ;print("\n") ;{while(field(fld,"\t\n" ," ,;\t" ) != ';'); continue;} ;
	}
	sp = up->ug_arglist;
	ug->arg_c = up->ug_call;

	while(bc = field(fld,"\t\n" ," ,;\t" )){
	    if(!  ug->arg_list )  ug->arg_list  =  uarg  = (struct    ug_adesc   *) calloc(1, sizeof(struct    ug_adesc  )) ; else{ for(  uarg  =   ug->arg_list  ;   uarg  ->   next_uarg  ;   uarg  =  uarg  ->   next_uarg  ) ;  uarg ->  next_uarg  = (struct    ug_adesc   *) calloc(1, sizeof(struct    ug_adesc  )) ;  uarg  =  uarg ->  next_uarg ; } ;
	    if(sfield(&sp,sfld,"\t\n" ,"[")== ((void *)0) ){
	     { sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Excess") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ;print("\n") ;break;
	    }
	    sfield(&sp,sfld,"\t\n" ,"]");
	    if(*sp == '*')while(*sp-- != '[');
	    ug->n_arg++;
	    if ( (__ctype_b[(int) ( (  fld[1]  ) )] & (unsigned short int)   _ISdigit )   )
		uarg->a_index = atoi(&fld[1]) ;
	    else if ( fld[1] == '[' ) {
 


		*((char *)rindex(fld, (int) ']' )) = ' ' ;
		uarg->a_index = expr( &fld[2] ) ;
	    }

printf("fld[0] =  %s\n", fld);
	    switch(uarg->a_type = fld[0]){
	     case 'b': ins->Nb++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(uarg->a_index > ins->bmax) ins->bmax=uarg->a_index;
		if(ins->bmax > Bmax){Bmax = ins->bmax; Bextend = 1;}
printf("Here is the error sfld = %s\n", sfld);
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 'v': ins->Nv++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(uarg->a_index > ins->vmax) ins->vmax=uarg->a_index;
		if(ins->vmax > Vmax){Vmax = ins->vmax; Vextend = 1;}
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 'd': ins->Nd++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(! __extension__	({ size_t __s1_len, __s2_len;	(__builtin_constant_p ( fld ) && __builtin_constant_p ( "d" )	&& (__s1_len = strlen ( fld ), __s2_len = strlen ( "d" ),	(! ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1)  || __s1_len >= 4)	&& (! ((size_t)(const void *)((  "d"  ) + 1) - (size_t)(const void *)(  "d"  ) == 1)  || __s2_len >= 4))	? memcmp ((__const char *) ( fld ), (__const char *) ( "d" ),	(__s1_len < __s2_len ? __s1_len : __s2_len) + 1)	: (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	&& (__s1_len = strlen ( fld ), __s1_len < 4)	? (__builtin_constant_p ( "d" ) && ((size_t)(const void *)((  "d"  ) + 1) - (size_t)(const void *)(  "d"  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   "d"  ))[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[1]);	if (  __s1_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[2]);	if (  __s1_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s2 =	(__const unsigned char *) (__const char *) (   "d"  );	register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - __s2[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1] - __s2[1]); if (  __s1_len  > 1 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[2] - __s2[2]);	if (  __s1_len  > 2 && __result == 0)	__result = (((__const unsigned char *)	(__const char *)  (  fld  ))[3]	- __s2[3]);	}	}	__result; })) )	: (__builtin_constant_p ( "d" ) && ((size_t)(const void *)((  "d"  ) + 1) - (size_t)(const void *)(  "d"  ) == 1) 	&& (__s2_len = strlen ( "d" ), __s2_len < 4)	? (__builtin_constant_p ( fld ) && ((size_t)(const void *)((  fld  ) + 1) - (size_t)(const void *)(  fld  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  fld  ))[0] - ((__const unsigned char *) (__const char *)(   "d"  ))[0]);	if (  __s2_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  fld  ))[1]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[2]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  fld  ))[3]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s1 =	(__const unsigned char *) (__const char *) (  fld  );	register int __result =	__s1[0] - ((__const unsigned char *)	(__const char *) (   "d"  ))[0];	if (  __s2_len  > 0 && __result == 0)	{	__result = (__s1[1]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(__s1[2] - ((__const unsigned char *)	(__const char *) (   "d"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(__s1[3]	- ((__const unsigned char *)	(__const char *) (   "d"  ))[3]);	}	}	__result; })) )	: strcmp ( fld ,  "d" )))); }) ) uarg->a_index = ++ins->dmax;
		if(uarg->a_index > ins->dmax) ins->dmax=uarg->a_index;
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 'p': ins->Np++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(uarg->a_index > ins->pmax) ins->pmax=uarg->a_index;
		if(ins->pmax > Pmax){Pmax = ins->pmax; Pextend = 1;}
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 's': ins->Ns++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(uarg->a_index > ins->smax) ins->smax=uarg->a_index;
		if(ins->smax > Strmax){Strmax = ins->smax; Sextend = 1;}
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 'f': ins->Nf++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(uarg->a_index > ins->fmax) ins->fmax=uarg->a_index;
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     case 'c': ins->Nc++; 
		if(Barefile)fprintf(Barefile, "%s " , fld ); ;
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ; break;
	     default:  
		ins->Nn++;
		if(  ins->n_list )   ins->n_list  = (  float  *) realloc(  ins->n_list ,(  ins->Nn+1 )*sizeof(  float )); else   ins->n_list  = (  float  *) malloc((  ins->Nn+1 )*sizeof(  float ));  nptr  = (  float  *) (  ins->n_list +(  ins->Nn+1 )-1) ;
		if( index(fld,'b') || index(fld,'p') ){
		    Error++;
		    { sprintf(Junk, "Illegal expr in ug field: '%s' " , fld ); print(Junk); } ;
		}
		*nptr = expr(fld);
		uarg->a_type = 'n';
		uarg->a_index = ins->Nn;
		if(!index(sfld,'x')){printf("index returned %d\n", index(sfld,'x')); Error++;{ sprintf(Junk," Unit Generator '%s'", up->ug_name ); print(Junk); } ;print(" Illegal") ;{ sprintf(Junk," Field '%s'", fld ); print(Junk); } ; print("\n") ;{ sprintf(Junk," Usage: '%s'\n", up->ug_arglist ); print(Junk); } ;continue;} ;
		}
	    if(bc == ';')break;
	}  
	if(Barefile)fprintf(Barefile, ";\n" ); ;
	if(! __extension__	({ size_t __s1_len, __s2_len;	(__builtin_constant_p ( up->ug_name ) && __builtin_constant_p (  "out" )	&& (__s1_len = strlen ( up->ug_name ), __s2_len = strlen (  "out" ),	(! ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1)  || __s1_len >= 4)	&& (! ((size_t)(const void *)((   "out"  ) + 1) - (size_t)(const void *)(   "out"  ) == 1)  || __s2_len >= 4))	? memcmp ((__const char *) ( up->ug_name ), (__const char *) (  "out" ),	(__s1_len < __s2_len ? __s1_len : __s2_len) + 1)	: (__builtin_constant_p ( up->ug_name ) && ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1) 	&& (__s1_len = strlen ( up->ug_name ), __s1_len < 4)	? (__builtin_constant_p (  "out" ) && ((size_t)(const void *)((   "out"  ) + 1) - (size_t)(const void *)(   "out"  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  up->ug_name  ))[0] - ((__const unsigned char *) (__const char *)(    "out"  ))[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  up->ug_name  ))[1]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[1]);	if (  __s1_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  up->ug_name  ))[2]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[2]);	if (  __s1_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  up->ug_name  ))[3]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s2 =	(__const unsigned char *) (__const char *) (    "out"  );	register int __result =	(((__const unsigned char *) (__const char *) (  up->ug_name  ))[0] - __s2[0]);	if (  __s1_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  up->ug_name  ))[1] - __s2[1]); if (  __s1_len  > 1 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  up->ug_name  ))[2] - __s2[2]);	if (  __s1_len  > 2 && __result == 0)	__result = (((__const unsigned char *)	(__const char *)  (  up->ug_name  ))[3]	- __s2[3]);	}	}	__result; })) )	: (__builtin_constant_p (  "out" ) && ((size_t)(const void *)((   "out"  ) + 1) - (size_t)(const void *)(   "out"  ) == 1) 	&& (__s2_len = strlen (  "out" ), __s2_len < 4)	? (__builtin_constant_p ( up->ug_name ) && ((size_t)(const void *)((  up->ug_name  ) + 1) - (size_t)(const void *)(  up->ug_name  ) == 1) 	? (__extension__ ({ register int __result =	(((__const unsigned char *) (__const char *) (  up->ug_name  ))[0] - ((__const unsigned char *) (__const char *)(    "out"  ))[0]);	if (  __s2_len  > 0 && __result == 0)	{	__result = (((__const unsigned char *)	(__const char *) (  up->ug_name  ))[1]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(((__const unsigned char *)	(__const char *) (  up->ug_name  ))[2]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(((__const unsigned char *)	(__const char *) (  up->ug_name  ))[3]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[3]);	}	}	__result; })) 	: (__extension__ ({ __const unsigned char *__s1 =	(__const unsigned char *) (__const char *) (  up->ug_name  );	register int __result =	__s1[0] - ((__const unsigned char *)	(__const char *) (    "out"  ))[0];	if (  __s2_len  > 0 && __result == 0)	{	__result = (__s1[1]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[1]);	if (  __s2_len  > 1 && __result == 0)	{	__result =	(__s1[2] - ((__const unsigned char *)	(__const char *) (    "out"  ))[2]);	if (  __s2_len  > 2 && __result == 0)	__result =	(__s1[3]	- ((__const unsigned char *)	(__const char *) (    "out"  ))[3]);	}	}	__result; })) )	: strcmp ( up->ug_name ,   "out" )))); })  && ug->n_arg != Nchan){
	    Error++;
	    sprintf(Junk,"%d channels but %d inputs to 'out'\n",
		Nchan, ug->n_arg);
	    print(Junk);
	}
    }  
    

    if(Bextend){
	if(  Blocks )   Blocks  = (  float  *) realloc(  Blocks ,(  (Bmax+1)*Lblocks )*sizeof(  float )); else   Blocks  = (  float  *) malloc((  (Bmax+1)*Lblocks )*sizeof(  float ));  Bp  = (  float  *) (  Blocks +(  (Bmax+1)*Lblocks )-1) ;
    }
    if(Vextend){
	if(  Vars )   Vars  = (  float  *) realloc(  Vars ,(  Vmax+1 )*sizeof(  float )); else   Vars  = (  float  *) malloc((  Vmax+1 )*sizeof(  float ));  Vp  = (  float  *) (  Vars +(  Vmax+1 )-1) ;
    }
    if(Pextend){
	if(  Params )   Params  = (  float  *) realloc(  Params ,(  Pmax+1 )*sizeof(  float )); else   Params  = (  float  *) malloc((  Pmax+1 )*sizeof(  float ));  Pp  = (  float  *) (  Params +(  Pmax+1 )-1) ;
    }
    if(Sextend){
	if(Strvars) Strvars = 
	 (char **) realloc(Strvars,(Strmax+1)*sizeof(char **));
	else 
	 Strvars = 
	  (char **) malloc((Strmax+1)*sizeof(char **)); 
	 Strp = (Strvars+Strmax);
    }
}
