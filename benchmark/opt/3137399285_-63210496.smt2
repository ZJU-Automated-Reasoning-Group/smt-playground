(declare-fun k!1729 () (_ BitVec 8))
(assert (= k!1729 #x3c))

(maximize k!1729) 
(set-option :opt.priority box) 
(check-sat)
