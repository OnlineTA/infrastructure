PDF_FILES = $(wildcard figures/*.pdf)
#SVG_FILES_2 = $(wildcard graphs/*.svg)
CROP_FILES = $(addprefix figures/,$(notdir $(PDF_FILES:-in.pdf=-crop.pdf)))
#PDF_FILES += $(addprefix graphs/,$(notdir $(SVG_FILES_2:.svg=.pdf))) 

all:svgfigures

svgfigures:$(CROP_FILES)

figures/%-crop.pdf:figures/%-in.pdf
	pdfcrop $<

#figures/%.pdf:figures/%.svg
#		inkscape -z -D --export-pdf=$@ $<
#graphs/%.pdf:graphs/%.svg
#		inkscape -z -D --export-pdf=$@ $<
