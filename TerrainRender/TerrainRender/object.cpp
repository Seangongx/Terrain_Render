#include "object.h"


//Node::Node(
//	GLint _cn_x, GLint _cn_y,
//	GLint _ctr_x, GLint _ctr_y,
//	GLint _ctl_x, GLint _ctl_y,
//	GLint _cbl_x, GLint _cbl_y,
//	GLint _cbr_x, GLint _cbr_y,
//	GLint _et_x, GLint _et_y,
//	GLint _eb_x, GLint _eb_y,
//	GLint _el_x, GLint _el_y,
//	GLint _er_x, GLint _er_y
//)
//{
//	center.cor_x = _cn_x; center.cor_y = _cn_y;
//	corner_top_right.cor_x = _ctr_x; corner_top_right.cor_y = _ctr_y;
//	corner_top_left.cor_x = _ctl_x; corner_top_left.cor_y = _ctl_y;
//	corner_bottom_left.cor_x = _cbl_x; corner_bottom_left.cor_y = _cbl_y;
//	corner_bottom_right.cor_x = _cbr_x; corner_bottom_right.cor_y = _cbr_y;
//	edge_top.cor_x = _et_x; edge_top.cor_y = _et_y;
//	edge_bottom.cor_x = _eb_x; edge_bottom.cor_y = _eb_y;
//	edge_left.cor_x = _el_x; edge_left.cor_y = _el_y;
//	edge_right.cor_x = _er_x; edge_right.cor_y = _er_y;
//}



Object::Object(float _x, float _y, float _width, float _height) :
	x(_x),
	y(_y),
	width(_width),
	height(_height)
{
}
Object::~Object()
{
}
