package antsqueue

type userRequestList struct {
	request chan func()
	UID     string
}

func newUserRequestList(sz int) *userRequestList {
	return &userRequestList{
		request: make(chan func(), sz),
	}
}
