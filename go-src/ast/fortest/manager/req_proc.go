package fortest


func WaitReqDone(cmd string){
	reqMetaList[cmd].wg.Wait()
}
