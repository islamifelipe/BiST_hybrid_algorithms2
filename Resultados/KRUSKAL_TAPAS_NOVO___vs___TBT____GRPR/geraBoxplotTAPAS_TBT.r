 #!/usr/bin/Rscript



# jpeg('TAPAS_vs_TBT/epsilon_100_corr3.jpg')
# tapas_eps <- read.table(file="eps/correlated/100.corr3/resultadoeps_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="eps/correlated/100.corr3/resultadoeps_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("TAPAS", "T-BT"))
# dev.off()

# jpeg('TAPAS_vs_TBT/hypervolume_100_corr3.jpg')
# tapas_eps <- read.table(file="hypervolume/correlated/100.corr3/resultadohyp_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="hypervolume/correlated/100.corr3/resultadohyp_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("TAPAS", "T-BT"))
# dev.off()


# jpeg('TAPAS_vs_TBT/r2_100_corr3.jpg')
# tapas_eps <- read.table(file="R2/correlated/100.corr3/resultador_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="R2/correlated/100.corr3/resultador_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT"))
# dev.off()





# jpeg('TAPAS_vs_TBT/epsilon_50_anticorr3.jpg')
# tapas_eps <- read.table(file="eps/anticorrelated/50.anticorr3/resultadoeps_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="eps/anticorrelated/50.anticorr3/resultadoeps_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("TAPAS", "T-BT"))
# dev.off()

# jpeg('TAPAS_vs_TBT/hypervolume_50_anticorr3.jpg')
# tapas_eps <- read.table(file="hypervolume/anticorrelated/50.anticorr3/resultadohyp_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="hypervolume/anticorrelated/50.anticorr3/resultadohyp_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("TAPAS", "T-BT"))
# dev.off()


# jpeg('TAPAS_vs_TBT/r2_50_anticorr3.jpg')
# tapas_eps <- read.table(file="R2/anticorrelated/50.anticorr3/resultador_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="R2/anticorrelated/50.anticorr3/resultador_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT"))
# dev.off()











# jpeg('TAPAS_vs_TBT/epsilon_200_anticorr1.jpg')
# tapas_eps <- read.table(file="eps/anticorrelated/200.anticorr1/resultadoeps_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="eps/anticorrelated/200.anticorr1/resultadoeps_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("TAPAS", "T-BT"))
# dev.off()

jpeg('TAPAS_vs_TBT/hypervolume_200_anticorr1.jpg')
tapas_eps <- read.table(file="hypervolume/anticorrelated/200.anticorr1/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/anticorrelated/200.anticorr1/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("TAPAS", "T-BT"), ylim = c(196000, 250000))
dev.off()


# jpeg('TAPAS_vs_TBT/r2_200_anticorr1.jpg')
# tapas_eps <- read.table(file="R2/anticorrelated/200.anticorr1/resultador_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="R2/anticorrelated/200.anticorr1/resultador_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT"))
# dev.off()










# jpeg('TAPAS_vs_TBT/epsilon_200_anticorr2.jpg')
# tapas_eps <- read.table(file="eps/anticorrelated/200.anticorr2/resultadoeps_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="eps/anticorrelated/200.anticorr2/resultadoeps_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("TAPAS", "T-BT"))
# dev.off()

# jpeg('TAPAS_vs_TBT/hypervolume_200_anticorr2.jpg')
# tapas_eps <- read.table(file="hypervolume/anticorrelated/200.anticorr2/resultadohyp_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="hypervolume/anticorrelated/200.anticorr2/resultadohyp_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("TAPAS", "T-BT"))
# dev.off()


# jpeg('TAPAS_vs_TBT/r2_200_anticorr2.jpg')
# tapas_eps <- read.table(file="R2/anticorrelated/200.anticorr2/resultador_Tapas.out",header=FALSE,sep=",")
# tbt_eps <- read.table(file="R2/anticorrelated/200.anticorr2/resultador_TBT.out",header=FALSE,sep=",")
# boxplot(tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT"))
# dev.off()




