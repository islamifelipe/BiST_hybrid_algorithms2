 #!/usr/bin/Rscript


jpeg('epsilon_300.conc1.jpg')
grasp_eps <- read.table(file="eps/concave/300.conc1/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/300.conc1/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/300.conc1/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('epsilon_300.conc3.jpg')
grasp_eps <- read.table(file="eps/concave/300.conc3/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/300.conc3/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/300.conc3/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('epsilon_400.conc1.jpg')
grasp_eps <- read.table(file="eps/concave/400.conc1/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/400.conc1/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/400.conc1/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()

jpeg('epsilon_600.conc3.jpg')
grasp_eps <- read.table(file="eps/concave/600.conc3/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/600.conc3/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/600.conc3/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('epsilon_700.conc2.jpg')
grasp_eps <- read.table(file="eps/concave/700.conc2/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/700.conc2/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/700.conc2/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('epsilon_900.conc1.jpg')
grasp_eps <- read.table(file="eps/concave/900.conc1/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/900.conc1/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/900.conc1/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('epsilon_900.conc2.jpg')
grasp_eps <- read.table(file="eps/concave/900.conc2/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/900.conc2/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/900.conc2/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('epsilon_900.conc3.jpg')
grasp_eps <- read.table(file="eps/concave/900.conc3/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/900.conc3/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/900.conc3/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('epsilon_400.anticorr3.jpg')
grasp_eps <- read.table(file="eps/anticorrelated/400.anticorr3/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/anticorrelated/400.anticorr3/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/anticorrelated/400.anticorr3/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "épsilon", names = c("GRPR","TAPAS", "T-BT"))
dev.off()












## r2


jpeg('r2_300.conc1.jpg')
grasp_eps <- read.table(file="R2/concave/300.conc1/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/300.conc1/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/300.conc1/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", ylim = c(0, 1), names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('r2_300.conc3.jpg')
grasp_eps <- read.table(file="R2/concave/300.conc3/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/300.conc3/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/300.conc3/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('r2_400.conc1.jpg')
grasp_eps <- read.table(file="R2/concave/400.conc1/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/400.conc1/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/400.conc1/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()

jpeg('r2_600.conc3.jpg')
grasp_eps <- read.table(file="R2/concave/600.conc3/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/600.conc3/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/600.conc3/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('r2_700.conc2.jpg')
grasp_eps <- read.table(file="R2/concave/700.conc2/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/700.conc2/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/700.conc2/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('r2_900.conc1.jpg')
grasp_eps <- read.table(file="R2/concave/900.conc1/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/900.conc1/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/900.conc1/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('r2_900.conc2.jpg')
grasp_eps <- read.table(file="R2/concave/900.conc2/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/900.conc2/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/900.conc2/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('r2_900.conc3.jpg')
grasp_eps <- read.table(file="R2/concave/900.conc3/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/concave/900.conc3/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/concave/900.conc3/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('r2_400.anticorr3.jpg')
grasp_eps <- read.table(file="R2/anticorrelated/400.anticorr3/resultador_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="R2/anticorrelated/400.anticorr3/resultador_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="R2/anticorrelated/400.anticorr3/resultador_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "R2", names = c("GRPR","TAPAS", "T-BT"))
dev.off()





#hyp


jpeg('hypervolume_300.conc1.jpg')
grasp_eps <- read.table(file="hypervolume/concave/300.conc1/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/300.conc1/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/300.conc1/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('hypervolume_300.conc3.jpg')
grasp_eps <- read.table(file="hypervolume/concave/300.conc3/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/300.conc3/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/300.conc3/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()



jpeg('hypervolume_400.conc1.jpg')
grasp_eps <- read.table(file="hypervolume/concave/400.conc1/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/400.conc1/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/400.conc1/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()

jpeg('hypervolume_600.conc3.jpg')
grasp_eps <- read.table(file="hypervolume/concave/600.conc3/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/600.conc3/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/600.conc3/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('hypervolume_700.conc2.jpg')
grasp_eps <- read.table(file="hypervolume/concave/700.conc2/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/700.conc2/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/700.conc2/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('hypervolume_900.conc1.jpg')
grasp_eps <- read.table(file="hypervolume/concave/900.conc1/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/900.conc1/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/900.conc1/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('hypervolume_900.conc2.jpg')
grasp_eps <- read.table(file="hypervolume/concave/900.conc2/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/900.conc2/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/900.conc2/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('hypervolume_900.conc3.jpg')
grasp_eps <- read.table(file="hypervolume/concave/900.conc3/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/concave/900.conc3/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/concave/900.conc3/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


jpeg('hypervolume_400.anticorr3.jpg')
grasp_eps <- read.table(file="hypervolume/anticorrelated/400.anticorr3/resultadohyp_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="hypervolume/anticorrelated/400.anticorr3/resultadohyp_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="hypervolume/anticorrelated/400.anticorr3/resultadohyp_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps[,c("V1")],tapas_eps[,c("V1")], tbt_eps[,c("V1")], ylab = "hypervolume", names = c("GRPR","TAPAS", "T-BT"))
dev.off()




