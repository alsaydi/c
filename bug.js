function Point(x, y) {
    this.x = Math.round(x);
    this.y = Math.round(y);
    function _toString() {
        return "x: " + x.toString() + " y: " + y.toString();
    };
    this.toString = _toString;
}

function Bug(bugId, offsetFunction) {
    this._stop = false;
    this._points = new Array();
    this._moving = false;
    this.bug = document.getElementById(bugId);
    this.getOffset = offsetFunction;
    function _getX() {
        var cX = this.bug.getBoundingClientRect().left;
        return cX;
    }
    this.getX = _getX;
    function _getY() {
        var cY = this.bug.getBoundingClientRect().top;
        return cY;
    };
    this.getY = _getY;
    function _move(x, y) {
        bug.style.left = Math.round(x).toString() + "px";
        bug.style.top = Math.round(y).toString() + "px";
    }
    this.move = _move;
    function _findX1(slope, x2, y1, y2) {
        var x1 = (y1 - y2 + slope * x2) / slope;
        return x1;
    }
    function _moveTo(x, y, slope) {
        _moving = true;
        var cX = this.getX();
        var cY = this.getY();
        if (cX == x && cY == y) {
            //console.log("arrived at %d %d", x, y);
            _moving = false;
            return true;
        }
        var newX = 0;
        var newY = 0;
        var offset = this.getOffset();
        if (cY != y) {
            if (cY < y)
                newY = (cY + offset > y) ? y : cY + offset;
            else if (cY > y)
                newY = (cY - offset < y) ? y : cY - offset;
            newX = _findX1(slope, x, newY, y);
        } else {
            if (cX < x) {
                newX = (cX + offset > x) ? x : cX + offset;
            } else if (cX > x) {
                newX = (cX - offset < x) ? x : cX - offset;
            }
            newY = y;
        }
        if (isNaN(newX)) {
            newX = x;
        }
        this.move(newX, newY);
        return false;
    };
    this.moveTo = _moveTo;
    function _findSlope(x1, y1, x2, y2) {
        var slope = 0;
        slope = ((y2 - y1) / (x2 - x1));
        //console.log("findSlope: x1:%d y1:%d, x2:%d y2:%d slope:%d", x1, y1, x2, y2, slope);
        return slope;
    };
    function _startMoving(x, y) {
        this._moving = true;
        var cX = this.getX();
        var cY = this.getY();
        var slope = _findSlope(cX, cY, x, y);
        if (isNaN(slope)) {
            console.log("ok no need to move because slope isNaN");
            return false;
        }
        var intervalId = setInterval(function (pBug) {
            var arrived = pBug.moveTo(x, y, slope);
            if (arrived) {
                clearInterval(intervalId);
                pBug._moving = false;
            }
        }, 100,this);
    };
    this.startMoving = _startMoving;

    function _moveRandomly() {
        var x = Math.round((Math.random() * 500 + 1));
        var y = Math.round((Math.random() * 570 + 1));
        this._points.push(new Point(x, y));
        var queueIntervalId = setInterval(function (pBug) {
            if (!pBug.isMoving() && !pBug._stop) {
                var p = pBug._points.shift();
                pBug.startMoving(p.x, p.y);
            } else {
                var x = (Math.random() * 500 + 1);
                var y = (Math.random() * 570 + 1);
                pBug._points.push(new Point(x, y));
            }
        }, 1, this);
    };
    this.moveRandomly = _moveRandomly;
    function _isMoving() {
        return this._moving;
    };
    this.isMoving = _isMoving;
    function _stopMoving() {
        if (this.isMoving()) {
            this._stop = true;
        }
    }
    this.stopMoving = _stopMoving;
    function _resumeMoving() {
        if (!this.isMoving()) {
            this._stop = false;
        }
    }
    this.resumeMoving = _resumeMoving;
}
