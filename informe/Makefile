.PHONY: all

all: informe.pdf

informe.pdf: *.tex
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	pdflatex -interactive=nonstopmode -halt-on-error informe.tex

clean:
	rm -f *.pdf *.aux *.log
