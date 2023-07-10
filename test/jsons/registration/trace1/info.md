peer_ids:
  1.  2000000001 - **Source** chat with **bad title**
  2.  2000000001 - **Source** chat with **bad title**
  3.  2000000001 - **Source** chat with **title**
  4.  /del_source
  5.  2000000001 - **Source** chat without **title**
  
  6.  2000000100 - **Target** chat with **num** and **bad title**
  7.  2000000100 - **Target** chat with **bad num** and **title**
  8.  2000000100 - **Target** chat with **bad title**
  9.  2000000100 - **Target** chat with **bad num**
  10. 2000000100 - **Target** chat with **title**
  11. /del_target
  12. 2000000100 - **Target** chat with **num**
  13. /del_target
  14. 2000000100 - **Target** chat with **num** and **title**
  15. 2000000101 - **Target** chat with same **num** to check if error
  16. 2000000101 - **Target** chat
  17. 2000000102 - **Target** chat
 18.1 2000000103 - **Target** chat
 18.2 2000000103 - **Target** chat to check if error

  23. 999 - **Reg godlike** if already godlike
  24. 999 - **Reg godlike** without **num**
  26. 999 - **Reg godlike** with **bad num**
 26.1   2 - **Reg godlike** with **num 998** - error NOT FROM GODLIKE
 26.2 999 - **Reg godlike** with **num 998**

  27. 998 - **Del target by num** without **num**
  28. 998 - **Del target by num** with bad **num**
 29.1   2 - **Del target by num** with **num 23** - error NOT FROM GODLIKE
 29.2 998 - **Del target by num** with **num 23**

 30.0 998 - **Change mode**
  30.   2 - **Change mode** error NOT GODLIKE
  31. 999 - **Change mode**
  32. /del_source 2000000001
  33. 999 - **Change mode** error expected

  34. 2 - **Status**
  35. 5 - **Status**