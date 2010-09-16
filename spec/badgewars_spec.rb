require File.expand_path('../spec_helper', __FILE__)

describe 'BadgeWars' do
  before :each do
    @world = BadgeWars::World.new
  end
  
  it "has a default world state of 0" do
    BadgeWars.core_size.times do |n|
      @world.peek(n).should be_zero
    end
  end
end