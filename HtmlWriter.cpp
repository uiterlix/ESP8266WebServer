/*
 * HtmlWriter.cpp
 *
 *  Created on: 14 jan. 2017
 *      Author: xande
 */

#include "HtmlWriter.h"
#include <cstring>
#include <ESP8266WiFi.h>

#define LINEBREAK 1

HtmlWriter::HtmlWriter() {
	init();
}

HtmlWriter::~HtmlWriter() {
	cleanup();
}

HtmlWriter& HtmlWriter::startHtml() {
	append("<html>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::endHtml() {
	append("</html>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::startDiv(char* clazz) {
	append("<div");
	if (strlen(clazz) > 0) {
		append(" class=\"");
		append(clazz);
		append("\"");
	}
	append(">", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::endDiv() {
	append("</div>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::br() {
	append("<br/>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::text(char* text) {
	append(text, LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::text(float text) {
	char buf[4];
	dtostrf(text, 4, 2, buf);
	append(buf, LINEBREAK);
    return *this;
}

HtmlWriter& HtmlWriter::a(char* link, char* text) {
	append("<a href=\"");
	append(link);
	append("\">");
	append(text);
	append("</a>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::startStyle() {
	append("<style>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::endStyle() {
	append("</style>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::startBody() {
	append("<body>", LINEBREAK);
	return *this;
}

HtmlWriter& HtmlWriter::endBody() {
	append("</body>", LINEBREAK);
	return *this;
}

char* HtmlWriter::build() {
	return HtmlWriter::toString();
}

