 #!/usr/bin/Rscript



jpeg('R2_AVALIACOES_500_anticorr3.jpg')
tapas_R2 <- read.table(file="R2/anticorrelated/500.anticorr3/resultador_TAPAS.out",header=FALSE,sep=",")
tbt_R2 <- read.table(file="R2/anticorrelated/500.anticorr3/resultador_TBTiteracoes.out",header=FALSE,sep=",")
boxplot(tapas_R2[,c("V1")], tbt_R2[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT")) #ylim = c(196000, 250000)
dev.off()


jpeg('R2_AVALIACOES_600_anticorr3.jpg')
tapas_R2 <- read.table(file="R2/anticorrelated/600.anticorr3/resultador_TAPAS.out",header=FALSE,sep=",")
tbt_R2 <- read.table(file="R2/anticorrelated/600.anticorr3/resultador_TBTiteracoes.out",header=FALSE,sep=",")
boxplot(tapas_R2[,c("V1")], tbt_R2[,c("V1")], ylab = "R2", names = c("TAPAS", "T-BT")) # ylim = c(196000, 250000)
dev.off()


#Rscript