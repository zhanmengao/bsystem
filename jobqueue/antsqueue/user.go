package antsqueue

type UserRequestList struct {
	request chan func()
	UID     string
}

func NewUserRequestList(sz int) *UserRequestList {
	return &UserRequestList{
		request: make(chan func(), sz),
	}
}
