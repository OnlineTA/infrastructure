(TeX-add-style-hook
 "master"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("memoir" "a4paper" "11pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("fontenc" "T1") ("babel" "danish" "english") ("mathdesign" "urw-garamond") ("fixme" "status=draft") ("caption" "margin=10pt" "font=small" "labelfont=bf" "labelsep=endash") ("biblatex" "backend=biber" "backref=true" "style=numeric" "sorting=none" "citestyle=numeric") ("hyperref" "pdfauthor={Truls Asheim}" "pdftitle={Designing Infrastructure for an OnlineTA}" "colorlinks=true" "linkcolor=mycolor1" "citecolor=mycolor1" "urlcolor=mycolor1" "filecolor=mycolor1") ("cleveref" "noabbrev" "nameinlink")))
   (TeX-run-style-hooks
    "latex2e"
    "introduciton"
    "memoir"
    "memoir11"
    "inputenc"
    "fontenc"
    "babel"
    "mathdesign"
    "minted"
    "graphicx"
    "fixme"
    "amsthm"
    "xcolor"
    "amsmath"
    "wallpaper"
    "caption"
    "biblatex"
    "hyperref"
    "cleveref")
   (LaTeX-add-environments
    "property")
   (LaTeX-add-bibliographies
    "citations")))

