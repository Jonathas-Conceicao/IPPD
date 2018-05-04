#!/usr/bin/Rscript
library(ggplot2)

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
    pT[i, j+1] <- process[i, 2] ## Process time line
    cT[i, j+1] <- process[i, 3] ## Copy time line
    ## print(sprintf("%f %f", process[i,2], process[i,3]))
  }
  tT[1, j+1] <- total[1]
}

print (pT)
print (cT)
print (tT)

pdf ("test.pdf")
ggplot( pT, aes( y=(seq(0, 0.0002)) , x= seq(1,4) ) )  + geom_boxplot()
