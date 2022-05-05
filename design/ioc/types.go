package main

type ship struct {
	pulp *pulp
}

func NewShip(pulp *pulp) *ship {
	return &ship{
		pulp: pulp,
	}
}

type pulp struct {
	count int
}

func NewPulp() *pulp {
	return &pulp{}
}

func NewPulpCount(count int) *pulp {
	return &pulp{
		count: count,
	}
}
