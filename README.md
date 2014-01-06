# ChimpDB [![Build Status](https://travis-ci.org/tadasv/chimp.png?branch=master)](https://travis-ci.org/tadasv/chimp) #

I am looking for contributors. If anyone is interested to work on something crazy hit me up on twitter [@tadasv](http://twitter.com/tadasv) or [@chimpdb](http://twitter.com/chimpdb). If you find this project interesting/useful in any way and would like to help me out -- [tip me](https://www.gittip.com/tadasv/).

**This project is in a VERY early stage**

ChimpDB is a distributed database with built-in machine learning capabilities. In order to achieve the best possible performance, ChimpDB does not rely on any exising database technologies. The entire stack (storage,
replication, networking, api) will be built from scratch.

## Building ##

ChimpDB comes with [Vagrant](http://www.vagrantup.com "Vagrant") configuration file. The code has been tested on Ubuntu Presice 64bit machine. After launching a Vagrant machine all build dependencies will be installed, to build:

    cd /vagrant
    ./autogen.sh
    ./configure
    make
    make test (optional)
    make install (optional)
  
## Running ##

Launching ChimpDB is as easy as running `chimp` in your shell.


## Example ##

Suppose we want to perform perform linear regression on the famous iris dataset. The following is a python code
example:

```python
>>> import pychimp
>>> c = pychimp.Client(('localhost', 8000))    # instantiate ChimpDB client
```
    
Suppose we only have setosa class:

```python
>>> data = [
# sepalLength, sepalWidth, petalLength, petalWidth, species
5.1,3.5,1.4,0.2,1.0,
4.9,3.0,1.4,0.2,1.0,
4.7,3.2,1.3,0.2,1.0,
4.6,3.1,1.5,0.2,1.0,
5.0,3.6,1.4,0.2,1.0,
5.4,3.9,1.7,0.4,1.0,
4.6,3.4,1.4,0.3,1.0,
5.0,3.4,1.5,0.2,1.0,
4.4,2.9,1.4,0.2,1.0,
4.9,3.1,1.5,0.1,1.0,
5.4,3.7,1.5,0.2,1.0,
4.8,3.4,1.6,0.2,1.0,
4.8,3.0,1.4,0.1,1.0,
4.3,3.0,1.1,0.1,1.0,
5.8,4.0,1.2,0.2,1.0,
5.7,4.4,1.5,0.4,1.0,
5.4,3.9,1.3,0.4,1.0,
5.1,3.5,1.4,0.3,1.0,
5.7,3.8,1.7,0.3,1.0,
5.1,3.8,1.5,0.3,1.0,
5.4,3.4,1.7,0.2,1.0,
5.1,3.7,1.5,0.4,1.0,
4.6,3.6,1.0,0.2,1.0,
5.1,3.3,1.7,0.5,1.0,
4.8,3.4,1.9,0.2,1.0,
5.0,3.0,1.6,0.2,1.0,
5.0,3.4,1.6,0.4,1.0,
5.2,3.5,1.5,0.2,1.0,
5.2,3.4,1.4,0.2,1.0,
4.7,3.2,1.6,0.2,1.0,
4.8,3.1,1.6,0.2,1.0,
5.4,3.4,1.5,0.4,1.0,
5.2,4.1,1.5,0.1,1.0,
5.5,4.2,1.4,0.2,1.0,
4.9,3.1,1.5,0.1,1.0,
5.0,3.2,1.2,0.2,1.0,
5.5,3.5,1.3,0.2,1.0,
4.9,3.1,1.5,0.1,1.0,
4.4,3.0,1.3,0.2,1.0,
5.1,3.4,1.5,0.2,1.0,
5.0,3.5,1.3,0.3,1.0,
4.5,2.3,1.3,0.3,1.0,
4.4,3.2,1.3,0.2,1.0,
5.0,3.5,1.6,0.6,1.0,
5.1,3.8,1.9,0.4,1.0,
4.8,3.0,1.4,0.3,1.0,
5.1,3.8,1.6,0.2,1.0,
4.6,3.2,1.4,0.2,1.0,
5.3,3.7,1.5,0.2,1.0,
5.0,3.3,1.4,0.2,1.0,
]
```

Load the data into ChimpDB:

```python
# Create a new dataset
>>> c.dsnew("iris", 5)
>>> c.dsappend("iris", data)
```

Next, let's create two linear regression models one to model sepal length with sepal width, and another
one to model sepal length using petal width.

```python
>>> c.modnew("model1", "linear_regression")
>>> c.modnew("model2", "linear_regression")
```

Now build the models

```python
>>> c.modbuild("model1", {"dataset_name": "iris", "features": [1], "response": 0})
>>> c.modbuild("model2", {"dataset_name": "iris", "features": [3], "response": 0})
```

Predicting is very simple:

```python
>>> c.modpredict("model1", [3.4, 4.1])
[200, {'responses': [4.993564620877732, 5.477162697854874]}, None]
```

In the example above we are trying to predict two data points: 3.4 and 4.1.

## Interfacing With ChimpDB ##

ChimpDB works over TCP/IP and uses [MessagePack](http://www.msgpack.org/) for data transport.
MessagePack provides small traffic footprint and is relativelly easy to parse. Writing client code
for connecting to ChimpDB should be a breeze in any language.

### The Protocol ###

ChimpDB uses message based protocol. Each message is an array of items.

Every request message consists of atleast 1 item -- a command name, e.g:

    ["PING"]
    ["DSNEW", "dataset_name", 5]
    
In the example above we see two commands: ``PING`` and ``DSNEW``. ``PING`` command takes no arguments
where as ``DSNEW`` takes two additional parameters. Response messages are also arrays, but of fixed 3 item size:

    [200, None, None]
    [400, None, "error message"]
    
First item in the response list is response code. Response codes fall into 3 categories: success (`2xx`),
user error (`4xx`) and server error (`5xx`). Second response argument is the response data, if any. Final
element is the error message.


### Commands ###

* `PING` - Pings the server, a noop command.
* `SHUTDOWN` - Shuts down the server.
* `DSNEW <dataset name> <num columns>` - Creates a new dataset with the specified number of columns.
* `DSLIST` - Lists all available datasets.
* `DSAPPEND <dataset name> <data>` - Append data to the existing dataset. Data is a list of numbers and must be column aligned.
* `MODNEW <model name> <model type>` - Create a new model. Currently only ``linear_regression`` is a valid model type. 
* `MODBUILD <model name> [<model specific parameters>]` - Build given model.
* `MOPREDICT <model name> [model specific data]` - Uses given model to make a prediction.
