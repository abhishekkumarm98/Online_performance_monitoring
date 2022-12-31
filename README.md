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
* CARLSim 5.0.0

## To run

```
Step 1 : To generate logs, go to Logs_script/ directory and execute each file in CARLSim. 
```

```
Step 2 : Create a directory for each crossbar size (32x32, 64x64, and 128x128) and save the generated logs there.
```

```
Step 3 : Then run the main inynb file to preprocess the logs and train the models.
```

```
Step 4 : To generate logs with single stuck at fault that manifest as missing/delayed and spurious pikes,
	  go to Inject_faults/ directory and execute each file within each directory in CARLsim.
```

```
Step 4 : Then run the preprocess inynb file to preprocess the unseen logs to evaluate.
```

```
Step 5 : Then get back to main inynb file for testing.
```



### To do
Need to upload few more files soon.
