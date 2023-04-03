# Online_performance_monitoring
# ETS 2023 : Online Performance Monitoring of Neuromorphic Computing Systems

## Envrionment

* Python 3.7.13
* Numpy 1.21.5
* Pandas 1.3.5
* Matplotlib 3.2.2
* BeautifulSoup 4.6.3
* Tensorflow 2.8.0
* Sklearn 1.0.2
* Xgboost 0.90
* Catboost 1.0.5
* Lightgbm 2.2.3
* CARLSim 6.0.0

## To run

```
Step 1 : To generate logs, go to Logs_script/ directory and execute each file in CARLSim. 
```

```
Step 2 : Create a directory for each crossbar size (32x32, 64x64, and 128x128) and save the generated logs file there under
	 directories (Output_32x32_precise_1k_logs, Output_64x64_precise_1k_logs, Output_128x128_precise_1k_logs) respectively.
```

```
Step 3 : Then run the "Main" inynb file to preprocess the logs and train the models.
```

```
Step 4 : To generate logs with single stuck at fault that manifest as missing/delayed and spurious spikes,
	  go to Inject_faults/ directory and execute each file within each directory in CARLsim.
```

```
Step 5 : Then run the "Results" inynb file to preprocess the generated logs with single stuck at fault with different utilizations
	  and then get the fault detection rate.
```


