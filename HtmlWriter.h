/*
 * HtmlWriter.h
 *
 *  Created on: 14 jan. 2017
 *      Author: xande
 */
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <ESP8266WiFi.h>

#ifndef HTMLWRITER_H_
#define HTMLWRITER_H_

class HtmlWriter {
private:
	char* header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE HTML>\n";
	char* html = NULL;
	void init() {
		html = (char*) malloc(strlen(header) + 1);
		strcpy(html, header);
	}
	void cleanup() {
		free(html);
	}
	void append(char* content, int newLine = 0) {
		char* old = html;
		int newSize = strlen(html) + strlen(content) + 1;
		if (newLine)
			newSize++;
		char* buffer = (char *) malloc(newSize);
		strcpy(buffer, html);
		strcat(buffer, content);
		if (newLine)
			strcat(buffer, "\n");
		html = buffer;
		free(old);
	}
	char* toString() {
		return html;
	}
public:
	HtmlWriter();
	virtual ~HtmlWriter();

	HtmlWriter& startHtml();
	HtmlWriter& endHtml();
	HtmlWriter& startBody();
	HtmlWriter& endBody();
	HtmlWriter& startDiv(char* clazz = "");
	HtmlWriter& endDiv();
	HtmlWriter& br();
	HtmlWriter& text(char* text);
	HtmlWriter& text(float text);
	HtmlWriter& startStyle();
	HtmlWriter& endStyle();
	char* build();
};

#endif /* HTMLWRITER_H_ */
