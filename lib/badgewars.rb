$:.push File.expand_path('../../ext', __FILE__)
require 'badgewars_ext'
require 'badgewars/version'

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