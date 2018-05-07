#!/usr/bin/Rscript
library(ggplot2)

n     <- as.integer(commandArgs(trailingOnly=TRUE)[1])
times <- as.integer(commandArgs(trailingOnly=TRUE)[2])
file  <- commandArgs(trailingOnly=TRUE)[3]

pT <- data.frame()
cT <- data.frame()
tT <- data.frame()

## for (j in 0:(times - 1)) {
##   table <- read.csv(file, header=FALSE, nrows=n+1, skip=(j*(n+1)), )
##   process <- table[complete.cases(table),]
##   process <- process[order(process$V1),]
##   total   <- table[!complete.cases(table),1]
##   ## names(process) <- c("Process ID", "Process Time", "Copy Time")
##   ## names(total)   <- c("Total Time")
##   ## print(process)
##   for (i in 1:n) {
##     pT[(j+1)+((i-1) * times), 1] <- process[i, 2] ## Process time line
##     pT[(j+1)+((i-1) * times), 2] <- process[i, 3] ## Copy time line
##     pT[(j+1)+((i-1) * times), 3] <- i
##     ## cT[(j+1)+((i-1) * times), 3] <- i
##   }
##   tT[j+1, 1] <- total[1]
##   tT[j+1, 2] <- j
## }

table <- read.csv(file, header=FALSE)
pT    <- table[complete.cases(table),]
total <- as.data.frame(t(t(table[!complete.cases(table),1])))

## names(pT) <- c("Process Time","Process")
## names(cT) <- c("Copy Time","Process")
names(pT)    <- c("Process", "Process Time", "Copy Time")
names(total) <- c("Total Time")

## print (pT)
## print (total)

pdf ("test.pdf")
ggplot( pT, aes(x = Process, y = `Process Time`, group=Process)) +
  scale_x_continuous(name = "Numero de Processos MPI") +
  scale_y_continuous(name = "Tempo (s)") +
  ggtitle("Tempo de Processamento") +
  geom_boxplot() +
  stat_summary(fun.y=mean, geom="point", shape=8, size=5, color="red", fill="red")

ggplot( pT, aes(x = Process, y = `Copy Time`, group=Process)) +
  scale_x_continuous(name = "Numero de Processos MPI") +
  scale_y_continuous(name = "Tempo (s)") +
  ggtitle("Tempo de Processamento") +
  geom_boxplot() +
  stat_summary(fun.y=mean, geom="point", shape=8, size=5, color="red", fill="red")

ggplot( total, aes(x = 1, y = `Total Time`)) +
  scale_y_continuous(name = "Tempo (s)") +
  ggtitle("Tempo total") +
  geom_boxplot() +
  stat_summary(fun.y=mean, geom="point", shape=8, size=5, color="red", fill="red")

