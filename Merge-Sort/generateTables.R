#!/usr/bin/Rscript
library(ggplot2)
library(lattice)
library(plyr)
library(Rmisc)
library(scales)

n     <- as.integer(commandArgs(trailingOnly=TRUE)[1])
times <- as.integer(commandArgs(trailingOnly=TRUE)[2])
file  <- commandArgs(trailingOnly=TRUE)[3]

pT <- data.frame()
cT <- data.frame()
tT <- data.frame()

for (j in 0:(times - 1)) {
  table <- read.csv(file, header=FALSE, nrows=n+1, skip=(j*(n+1)), )
  process <- table[complete.cases(table),]
  process <- process[order(process$V1),]
  total   <- table[!complete.cases(table),1]
  ## names(process) <- c("Process ID", "Process Time", "Copy Time")
  ## names(total)   <- c("Total Time")
  ## print(process)
  for (i in 1:n) {
    pT[(j+1)+((i-1) * times), 1] <- process[i, 2] ## Process time line
    pT[(j+1)+((i-1) * times), 2] <- process[i, 3] ## Copy time line
    pT[(j+1)+((i-1) * times), 3] <- i
    ## cT[(j+1)+((i-1) * times), 3] <- i
  }
  tT[j+1, 1] <- total[1]
}

## names(pT) <- c("Process Time","Process")
## names(cT) <- c("Copy Time","Process")
names(pT) <- c("Process Time", "Copy Time", "Process")


## print (pT)
## print (cT)
## print (tT)


pdf ("test.pdf")
ggplot( pT, aes(x = Process, y = `Process Time`, group=Process, fill=Process)) +
  scale_x_discrete(name = "Numero de Processos MPI") +
  scale_y_continuous(name = "Tempo (s)") +
  labs(fill="Processos") +
  ggtitle("Tempo de Processamento") +
  geom_boxplot()

ggplot( pT, aes(x = Process, y = `Copy Time`, group=Process, fill=Process)) +
  scale_x_discrete(name = "Numero de Processos MPI") +
  scale_y_continuous(name = "Tempo (s)") +
  labs(fill="Processos") +
  ggtitle("Tempo de Processamento") +
  geom_boxplot()
