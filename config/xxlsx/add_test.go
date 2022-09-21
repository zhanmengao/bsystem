package main

import (
	"fmt"
	"github.com/xuri/excelize/v2"
	"testing"
)

func TestAddChart(t *testing.T) {
	categories := map[string]string{
		"A2": "Small", "A3": "Normal", "A4": "Large",
		"B1": "Apple", "C1": "Orange", "D1": "Pear"}
	values := map[string]int{
		"B2": 2, "C2": 3, "D2": 3, "B3": 5, "C3": 2, "D3": 4, "B4": 6, "C4": 7, "D4": 8}
	f := excelize.NewFile()
	for k, v := range categories {
		f.SetCellValue("Sheet1", k, v)
	}
	for k, v := range values {
		f.SetCellValue("Sheet1", k, v)
	}
	if err := f.AddChart("Sheet1", "E1", `{
        "type": "col3DClustered",
        "series": [
        {
            "name": "Sheet1!$A$2",
            "categories": "Sheet1!$B$1:$D$1",
            "values": "Sheet1!$B$2:$D$2"
        },
        {
            "name": "Sheet1!$A$3",
            "categories": "Sheet1!$B$1:$D$1",
            "values": "Sheet1!$B$3:$D$3"
        },
        {
            "name": "Sheet1!$A$4",
            "categories": "Sheet1!$B$1:$D$1",
            "values": "Sheet1!$B$4:$D$4"
        }],
        "title":
        {
            "name": "Fruit 3D Clustered Column Chart"
        }
    }`); err != nil {
		fmt.Println(err)
		return
	}
	// Save spreadsheet by the given path.
	if err := f.SaveAs("Book1.xlsx"); err != nil {
		fmt.Println(err)
	}
}

func TestAddPicture(t *testing.T) {
	f, err := excelize.OpenFile("Book1.xlsx")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer func() {
		// Close the spreadsheet.
		if err := f.Close(); err != nil {
			fmt.Println(err)
		}
	}()
	// Insert a picture.
	if err := f.AddPicture("Sheet1", "A2", "image.png", ""); err != nil {
		fmt.Println(err)
	}
	// Insert a picture to worksheet with scaling.
	if err := f.AddPicture("Sheet1", "D2", "image.jpg",
		`{"x_scale": 0.5, "y_scale": 0.5}`); err != nil {
		fmt.Println(err)
	}
	// Insert a picture offset in the cell with printing support.
	if err := f.AddPicture("Sheet1", "H2", "image.gif", `{
        "x_offset": 15,
        "y_offset": 10,
        "print_obj": true,
        "lock_aspect_ratio": false,
        "locked": false
    }`); err != nil {
		fmt.Println(err)
	}
	// Save the spreadsheet with the origin path.
	if err = f.Save(); err != nil {
		fmt.Println(err)
	}
}
