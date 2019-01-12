int
sys_oprireProces(struct proc * p, void * v, register_t * retval)
{
  struct sys_oprireProces_args * argumente = v;
  int pid_proces = SCARG(argumente,pidProces);
  struct sys_kill_args * argKill = 
  malloc(sizeof(struct sys_kill_args),M_TEMP,M_WAITOK);
  (*argKill).pid.le.datum = (*argKill).pid.be.datum = pid_proces;
  (*argKill).signum.le.datum = (*argKill).signum.be.datum = SIGSTOP;
  v=argKill;
  int ret = sys_kill(p,v,retval);
  if(ret==0)
  {
    printf("Am oprit un proces\n");
  }
  return 0;
}
int
sys_pornireProces(struct proc * p, void * v, register_t * retval)
{
  struct sys_pornireProces_args * argumente = v;
  int pid_proces = SCARG(argumente,pidProces);
  struct sys_kill_args * argKill = 
malloc(sizeof(struct sys_kill_args),M_TEMP,M_WAITOK);
  (*argKill).pid.le.datum = (*argKill).pid.be.datum = pid_proces;
  (*argKill).signum.le.datum = (*argKill).signum.be.datum = SIGCONT;
  v = argKill;
  int ret = sys_kill(p,v,retval);
  if(ret==0)
  {
    printf("Am pornit un proces\n");
  }
  return 0;
}
