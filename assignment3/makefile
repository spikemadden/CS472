FILE = assignment3

all: writeup program

writeup:
	latex $(FILE).tex
	dvips -R -Poutline -t letter $(FILE).dvi -o $(FILE).ps
	ps2pdf $(FILE).ps

program:
	gcc endian.c -g -std=c99 -o assignment3
	exec ./assignment3

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg assignment3
