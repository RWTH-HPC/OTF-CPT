module M_time
contains
subroutine usleep(wait)
use,intrinsic :: iso_c_binding, only: c_int
integer,intent(in) :: wait
integer(kind=c_int):: waited
interface
 function c_usleep(msecs) bind (C,name="usleep")
  import
  integer(c_int) :: c_usleep
  integer(c_int),intent(in),VALUE :: msecs
 end function c_usleep
end interface
 if(wait.gt.0)then
  waited=c_usleep(int(wait,kind=c_int))
 endif
end subroutine usleep
end module M_time