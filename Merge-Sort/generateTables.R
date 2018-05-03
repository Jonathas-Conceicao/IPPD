#!/usr/bin/Rscript
n     <- as.integer(commandArgs(trailingOnly=TRUE)[1])
times <- as.integer(commandArgs(trailingOnly=TRUE)[2])
file  <- commandArgs(trailingOnly=TRUE)[3]
for (i in 0:(times - 1)) {
  table <- read.csv(file, header=FALSE, nrows=n+1, skip=(i*(n+1)))
  process <- table[complete.cases(table),]
  total <- table[!complete.cases(table),1]
  names(process) <- c("Process ID", "Process Time", "Copy Time")
  names(total) <- c("Total Time")
  print(process)
  print(total)
}
