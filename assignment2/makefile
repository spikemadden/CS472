FILE = assignment2

all: writeup program

writeup:
	latex $(FILE).tex
	dvips -R -Poutline -t letter $(FILE).dvi -o $(FILE).ps
	ps2pdf $(FILE).ps

program:
	gcc $(FILE).c -g -lm -std=c99 -o assignment2
	exec ./assignment2

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg assignment2
