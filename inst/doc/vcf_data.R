## ----fig.cap="Cartoon representation of VCF file organization", echo=FALSE, fig.height=4, fig.width=4, fig.align='center'----
par(mar=c(0.1,0.1,0.1,0.1))
plot(c(0,5), c(0,5), type="n", frame.plot=FALSE, axes=FALSE, xlab="", ylab="")
rect(xleft=0, ybottom=4, xright=3, ytop=5)
rect(xleft=0, ybottom=0, xright=2, ytop=4)
rect(xleft=2, ybottom=0, xright=5, ytop=4)
text(1.5, 4.7, "Meta information", cex=1)
text(1.5, 4.4, "(@meta)", cex=1)
text(1.0, 2.5, "Fixed information", cex=1)
text(1.0, 2.2, "(@fix)", cex=1)
text(3.5, 2.5, "Genotype information", cex=1)
text(3.5, 2.2, "(@gt)", cex=1)
par(mar=c(5,4,4,2))

## -----------------------------------------------------------------------------
library(vcfR)
data(vcfR_example)
vcf

## ----echo=TRUE----------------------------------------------------------------
strwrap(vcf@meta[1:7])

## -----------------------------------------------------------------------------
queryMETA(vcf)

## -----------------------------------------------------------------------------
queryMETA(vcf, element = 'DP')

## -----------------------------------------------------------------------------
queryMETA(vcf, element = 'FORMAT=<ID=DP')

## ----echo=TRUE----------------------------------------------------------------
head(getFIX(vcf))

## ----echo=TRUE----------------------------------------------------------------
vcf@gt[1:6, 1:4]

## -----------------------------------------------------------------------------
head(vcf)

