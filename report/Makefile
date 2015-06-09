SVG_FILES = $(wildcard figures/*.svg)
SVG_FILES_2 = $(wildcard graphs/*.svg)
PDF_FILES = $(addprefix figures/,$(notdir $(SVG_FILES:.svg=.pdf)))
PDF_FILES += $(addprefix graphs/,$(notdir $(SVG_FILES_2:.svg=.pdf))) 

all:svgfigures

svgfigures:$(PDF_FILES)

figures/%.pdf:figures/%.svg
		inkscape -z -D --export-pdf=$@ $<
graphs/%.pdf:graphs/%.svg
		inkscape -z -D --export-pdf=$@ $<
