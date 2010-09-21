require File.expand_path('../../ext/badgewars', __FILE__)

class << BadgeWars::Op
  def [](op, lhs = 0, rhs = 0)
    case op
    when Symbol
      op = BadgeWars::Op.const_get(op.to_s.upcase)
    when Integer
    else raise ArgumentError, "invalid BadgeWars::Op: #{op.inspect}"
    end
    
    BadgeWars::Op.new(op, lhs, rhs)
  end
end