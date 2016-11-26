FILE = assignment3

all: writeup endian cache

writeup:
	latex $(FILE).tex
	bibtex $(FILE)
	latex $(FILE).tex
	latex $(FILE).tex
	dvips -R -Poutline -t letter $(FILE).dvi -o $(FILE).ps
	ps2pdf $(FILE).ps

endian:
	gcc endian.c -g -std=c99 -o endian
	exec ./endian

cache:
	gcc cache.c -g -std=c99 -o cache
	exec ./cache

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg endian cache
