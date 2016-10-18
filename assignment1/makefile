FILE = assignment1

all: writeup program

writeup:
	latex $(FILE).tex
	dvips -R -Poutline -t letter $(FILE).dvi -o $(FILE).ps
	ps2pdf $(FILE).ps

program:
	gcc assignment1.c cachetable.h -std=c99 -o assignment1

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg assignment1

.PHONY: all show clean ps pdf showps
