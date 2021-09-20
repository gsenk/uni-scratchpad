setwd('/Users/Gasper/Desktop/DN programiranje/Obdelava')
dat <- read.csv("Podatki.csv", header=TRUE, stringsAsFactors=FALSE, sep=";", dec=".", strip.white=TRUE)
names(dat)
head(dat)

title <- dat$title

cena <- dat$price_spletna
npopust <- dat$prihranek_tot
popust <- dat$prihranek_proc
ocena <- dat$ocena
nkomentarjev <- dat$stevilo_komentarjev


cena <- cena[cena>0]
npopust <- npopust[npopust>0]
popust <- popust[popust>0]
ocena <- ocena[ocena>0]
nkomentarjev <- nkomentarjev[nkomentarjev>0]

summary(ocena_je)
table(ocena_je)
summary(npopust)
table(npopust)
summary(nkomentarjev)
table(nkomentarjev)

cena <- as.numeric(cena)
summary(cena)

prva_cifra_cena <- cena  %/% 10^floor(log(cena, 10))
summary(prva_cifra_cena)
table(prva_cifra_cena)

library(lattice)
histogram(cena[cena<1000], main="Histogram cen", xlab="Cena [$]", ylab="Pogostost")
histogram(cena[cena<100], main="Histogram cen", xlab="Cena [$]", ylab="Pogostost")
bins=seq(0,9,by=1)
histogram(prva_cifra_cena, breaks=bins, main="Histogram prvih številk v ceni", xlab="Prva številka", ylab="Pogostost")
histogram(npopust[npopust<100], main="Histogram vrednosti popusta", xlab="Popust", ylab="Pogostost")
histogram(popust, main="Histogram procentualnih popustov", xlab="Popust v %", ylab="Pogostost")
histogram(ocena, main="Povpreène ocene artiklov", xlab="Ocena", ylab="Pogostost")
histogram(nkomentarjev, main="Število artiklov z doloèenim številom komentarjev", xlab="Komentarjev", ylab="Pogostost")
histogram(nkomentarjev[nkomentarjev<50], main="Število artiklov z doloèenim številom komentarjev", xlab="Komentarjev", ylab="Pogostost")

plot(cena[(ocena>0)&(cena>0)&(cena<1000)], ocena[(ocena>0)&(cena>0)&(cena<1000)], pch=16, cex=0.5, xlab="Cena", ylab="Ocena", main="Ocena v odvisnosti od cene" )
