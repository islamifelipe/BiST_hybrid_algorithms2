 #!/usr/bin/Rscript

k=300
i=1
jpeg('epsilon_300.conc1.jpg')
grasp_eps <- read.table(file="eps/concave/$k.conc$i/resultadoeps_GRASP.out",header=FALSE,sep=",")
tapas_eps <- read.table(file="eps/concave/$k.conc$i/resultadoeps_Tapas.out",header=FALSE,sep=",")
tbt_eps <- read.table(file="eps/concave/$k.conc$i/resultadoeps_TBT.out",header=FALSE,sep=",")
boxplot(grasp_eps,tapas_eps, tbt_eps, ylab = "eps", names = c("GRPR","TAPAS", "T-BT"))
dev.off()


quit()