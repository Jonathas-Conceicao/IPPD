#!/usr/bin/Rscript
library(ggplot2)

n     <- as.integer(commandArgs(trailingOnly=TRUE)[1])
times <- as.integer(commandArgs(trailingOnly=TRUE)[2])
file  <- commandArgs(trailingOnly=TRUE)[3]

pT <- data.frame()
cT <- data.frame()
tT <- data.frame()

table <- read.csv(file, header=FALSE)
pT    <- table[complete.cases(table),]
total <- table[!complete.cases(table),c(1,2)]

names(pT)    <- c("Process", "ID", "Process Time", "Copy Time")
names(total) <- c("Process", "Total Time")

print (pT)
print (total)

pdf ("Medidas.pdf")
## ggplot( pT, aes(x = Process, y = `Process Time`, group=Process)) +
##   scale_x_continuous(name = "Numero de Processos MPI") +
##   scale_y_continuous(name = "Tempo (s)") +
##   ggtitle("Tempo de Processamento") +
##   geom_boxplot() +
##   stat_summary(fun.y=mean, geom="point", shape=20, size=5, color="red", fill="red")

## ggplot( pT, aes(x = Process, y = `Copy Time`, group=Process)) +
##   scale_x_continuous(name = "Numero de Processos MPI") +
##   scale_y_continuous(name = "Tempo (s)") +
##   ggtitle("Tempo de Cópia") +
##   geom_boxplot() +
##   stat_summary(fun.y=mean, geom="point", shape=20, size=5, color="red", fill="red")

ggplot( total, aes(x = Process, y = `Total Time`, group=Process)) +
  scale_y_continuous(name = "Tempo (s)") +
  ggtitle("Tempo total") +
  geom_boxplot() +
  stat_summary(fun.y=mean, geom="point", shape=20, size=5, color="red", fill="red")

