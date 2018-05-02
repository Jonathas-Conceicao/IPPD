#!/usr/bin/Rscript
n     <- as.integer(commandArgs(trailingOnly=TRUE)[1])
times <- as.integer(commandArgs(trailingOnly=TRUE)[2])
file  <- commandArgs(trailingOnly=TRUE)[3]
for (i in 0:(times - 1)) {
	table <- read.csv(file, header=FALSE, nrows=n+1, skip=(i*(n+1)))
	names(table) <- c("Process ID", "Process Time", "Copy Time")
	print(head(table, -1))
	total <- tail(table, 1)[1,1]
	names(total) <- c("Total Time")
	print(total)
}