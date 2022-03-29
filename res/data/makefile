all: pkdd15-i/pkdd15-i.runs

pkdd15-i/pkdd15-i.runs: pkdd15-i/process.py pkdd15-i/train.csv
	python3 pkdd15-i/process.py pkdd15-i/train.csv > $@

pkdd15-i/train.csv: pkdd15-i/train.zip
	unzip -o -d pkdd15-i $<
	touch $@

pkdd15-i/train.zip:
	curl https://dl.dropboxusercontent.com/s/7culexod1kn5xxr/train.zip --output $@
