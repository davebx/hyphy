/*---------------------------------------------------------
output a postscript header; set page size
---------------------------------------------------------*/

function _HYPSPageHeader (_width,_height,_doc_title)
{
	
	_res_string = "";
	_res_string * 64;
	_res_string * "%!\n";
	_res_string * ("%% PS file " + _doc_title);
	GetString (_hpv,HYPHY_VERSION,0);
	GetString (_cdt,TIME_STAMP,0);
	_res_string * ("\n%% Generated by HYPHY v" + _hpv + " on GMT " + _cdt);
	_res_string * ("\n<< /PageSize ["+_width+" "+_height+ "] >> setpagedevice");
	_res_string * 0;
	return _res_string;
}

/*---------------------------------------------------------
set a font (face and size)
---------------------------------------------------------*/

function _HYPSSetFont (_font_face,_font_size)
{
	
	_res_string = "";
	_res_string * 64;
	_res_string * ("/"+_font_face+" findfont\n");
	_res_string * ("/fs "+_font_size+" def\nfs scalefont\nsetfont\n");
	_res_string * 0;
	return _res_string;
}

/*---------------------------------------------------------
generates string centering commands

x y (text) centertext - to output (text) so that its center is at (x,y)

x y w (text) scalecentertext - to output (text) so that its center is at (x,y) and is not wider than w

x y (text) righttext - to output (text) so that its right bottom edge is at (x,y)


---------------------------------------------------------*/

function _HYPSTextCommands (dummy)
{
	_res_string = "";
	_res_string * 64;
	_res_string * "/centertext {dup newpath 0 0 moveto false charpath closepath pathbbox pop exch pop exch sub 2 div 4 -1 roll exch sub 3 -1 roll newpath moveto show} def\n";
	_res_string * "/vcentertext {90 rotate centertext -90 rotate} def\n";
	_res_string * "/righttext  {dup newpath 0 0 moveto false charpath closepath pathbbox pop exch pop exch sub       4 -1 roll exch sub 3 -1 roll newpath moveto show} def\n";
	_res_string * "/vrighttext  {3 -2 roll moveto 90 rotate show -90 rotate} def\n";
	_res_string * ("/scalecentertext {dup newpath 0 0 moveto false charpath closepath pathbbox 3 -1 roll sub 3 1 roll exch sub dup 5 -1 roll exch div dup 1 lt " +
				   "{dup 3 1 roll mul 2 div 6 -1 roll exch sub 3 1 roll dup 3 1 roll mul 2 div 5 -1 roll exch sub exch true 4 2 roll} " + 
	               "{pop 2 div 5 -1 roll exch sub exch 2 div 4 -1 roll exch sub false 3 -2 roll} ifelse newpath moveto {matrix currentmatrix exch dup scale exch show setmatrix} {show}  ifelse } def\n");
	_res_string * 0;
	return _res_string;
}


/*---------------------------------------------------------

draw a sequence profile box: colored (and labeled) 
partitioned rectangles

+-------------+-----------+-------+
|  A (100bp)  | B (200bp) |A(100bp|
+-------------+-----------+-------+

takes dimensions _w and _h, font size _f an Nx2 matrix with partition 
width labels (1st row) and proportional widths (2nd row) and an Nx6 matrix 
with the colors (RGB for the background and RGB for the text)

---------------------------------------------------------*/

_hyDefaultPSColors =
{
{102,51,51}
{153,204,153}
{102,0,153}
{153,0,153}
{255,51,51}
{102,0,153}
{255,204,0}
{153,255,153}
{102,153,51}
{102,204,102}
{0,51,153}
{255,102,0}
{153,153,51}
{255,153,0}
{153,51,102}
{153,153,153}
{102,102,153}
{153,51,51}
{204,51,51}
{102,153,102}
}*(1/255);


function _HYPSLabeledBoxes (_x, _y, _f, _boxes, _colors)
{
	_res_string = "";
	_res_string * 64;
	_partCount = Rows(_boxes);
	if (_partCount)
	{
		if (Columns(_colors) == 6 && Rows(_colors) == _partCount)
		{
			_res_string * "currentrgbcolor\n";
			_currentX 			= 0;
			_textLocation 		= _y/2;
			
			for 	(_i = 0; _i < _partCount; _i = _i + 1)
			{
				_res_string * ("" + _colors[_i][0] + " " + _colors[_i][1] + " " + _colors[_i][2] + " setrgbcolor\n");
				_boxWidth = (0+_boxes[_i][1])*_x;
				_res_string * ("" + _currentX + " 0 " + _boxWidth + " " + _y + " rectfill\n");
				_res_string * ("" + _colors[_i][3] + " " + _colors[_i][4] + " " + _colors[_i][5] + " setrgbcolor\n");
				_res_string * ("" + (_currentX + _boxWidth/2) + " " + _textLocation + " " + _boxWidth + " (" + _boxes[_i][0] + ") scalecentertext\n");
				_currentX = _currentX + _boxWidth;
			}
		}
		_res_string * "0 0 0 setrgbcolor currentlinewidth 1 setlinewidth\n";
		_res_string * ("0 0 "+ (_x) + " " + (_y) + " rectstroke setlinewidth\n");
		_res_string * "setrgbcolor\n";
	}
	_res_string * 0;
	return _res_string;
}


