#define HTML(x, y) "<" + (x) + ">" + (y) + "</" + (x) + ">"
#define HTML_PAGE(x, y) HTML("HTML", HTML("HEAD",HTML("TITLE",x)) + HTML("BODY",HTML("H1",y)))

#define HREF(x,y) "<A HREF=" + x + ">" + y + "</A>"


